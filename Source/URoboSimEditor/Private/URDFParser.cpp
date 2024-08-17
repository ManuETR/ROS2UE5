// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "URDFParser.h"
#include "Conversions.h"
// #include "Paths.h"
#include "XmlFile.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Factories/FbxFactory.h"
#include "RStaticMeshEditUtils.h"

// Default constructor
FURDFParser::FURDFParser() :  AssetRegistryModule(FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry")))
{
    this->XmlFile=nullptr;
    this->bSDFLoaded=false;
    GetROSPackagePaths();
}

// Constructor with load from path
FURDFParser::FURDFParser(const FString& InFilename) : AssetRegistryModule(FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry")))
{
  this->XmlFile=nullptr;
  this->bSDFLoaded=false;
  GetROSPackagePaths();
  LoadSDF(InFilename);
}

// Destructor
FURDFParser::~FURDFParser()
{
  Clear();
}

// Load sdf from file
bool FURDFParser::LoadSDF(const FString& InFilename)
{
  AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
  // Make sure parser is clean
  Clear();

  XmlFile = new FXmlFile(InFilename);
  bSDFLoaded = IsValidSDF();
  SetDirectoryPath(InFilename);
  FbxFactory = NewObject<UFbxFactory>(UFbxFactory::StaticClass());
  FbxFactory->EnableShowOption();

  return bSDFLoaded;
}

// Clear parser
void FURDFParser::Clear()
{
  if (XmlFile)
    {
      XmlFile->Clear();
      delete XmlFile;
      XmlFile = nullptr;
    }
  if (bSDFLoaded)
    {
      bSDFLoaded = false;
      DataAsset = nullptr;
      FbxFactory = nullptr;
      DirPath = TEXT("");
    }
}

// Create data asset and parse sdf data into it
USDFDataAsset* FURDFParser::ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags)
{
  if (!bSDFLoaded)
    {
      return nullptr;
    }

  // Create a new SDFDataAsset
  DataAsset = NewObject<USDFDataAsset>(InParent, InName, InFlags);

  // Parse sdf data and fill the data asset
  ParseSDF();

  return DataAsset;
}

// Check if sdf data is valid
bool FURDFParser::IsValidSDF() {
  if (XmlFile == nullptr) {
    return false;
  }

  // Check if root node is <sdf> or <gazebo> (sdf version 1.2)
  if (!XmlFile->GetRootNode()->GetTag().Equals(TEXT("robot"))) {
    UE_LOG(LogTemp, Error, TEXT("[%s][%d] Root node is not <robot>"), TEXT(__FUNCTION__), __LINE__);
    return false;
  }
  return true;
}

// Parse <robot> node
void FURDFParser::ParseSDF() {
  DataAsset->Version = TEXT("__default__");

  USDFModel* NewModel = nullptr;

  // Get "name" from node attribute
  const FString Name = XmlFile->GetRootNode()->GetAttribute(TEXT("name"));
  if(!Name.IsEmpty()) {
    NewModel = NewObject<USDFModel>(DataAsset, FName(*Name));
    NewModel->Name = Name;
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <model> has no \"name\" attribute, added a default value.."), *FString(__FUNCTION__), __LINE__);
    NewModel = NewObject<USDFModel>(DataAsset/*, FName(TEXT("__default__"))*/);
    NewModel->Name = TEXT("__default__");
  }
 
  // Iterate <robot> child nodes
  for (const auto& ChildNode : XmlFile->GetRootNode()->GetChildrenNodes()) {
    if (ChildNode->GetTag().Equals(TEXT("link"))) {
      ParseLink(ChildNode, NewModel);
    } else if (ChildNode->GetTag().Equals(TEXT("joint"))) {
      ParseJoint(ChildNode, NewModel);
    } else {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <robot> child <%s> not supported, ignored.."), *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
      continue;
    }
  }

  DataAsset->Models.Add(NewModel);
}

// Parse <link> node
void FURDFParser::ParseLink(const FXmlNode* InNode, USDFModel*& OutModel) {
  // Ptr to the new link
  USDFLink* NewLink = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty()) {
    NewLink = NewObject<USDFLink>(OutModel, FName(*Name));
    NewLink->Name = Name;
    CurrentLinkName = Name;
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> has no \"name\" attribute, added a default value.."), *FString(__FUNCTION__), __LINE__);
    NewLink = NewObject<USDFLink>(OutModel/*, FName(TEXT("__default__"))*/);
    NewLink->Name = TEXT("__default__");
  }

  // Iterate <link> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes()) {
    if (ChildNode->GetTag().Equals(TEXT("pose"))) {
      NewLink->Pose = PoseContentToFTransform(ChildNode->GetAttribute(TEXT("xyz")) + " " + ChildNode->GetAttribute(TEXT("rpy")));
    } else if (ChildNode->GetTag().Equals(TEXT("inertial"))) {
      ParseLinkInertial(ChildNode, NewLink);
    } else if (ChildNode->GetTag().Equals(TEXT("visual"))) {
      ParseVisual(ChildNode, NewLink);
    } else if (ChildNode->GetTag().Equals(TEXT("collision"))) {
      ParseCollision(ChildNode, NewLink);
    } else {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> child <%s> not supported, ignored.."), *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
      continue;
    }
  }
  if(NewLink->Collisions.Num() == 0)
    {
      USDFCollision* Collision = CreateVirtualCollision(NewLink);
      if(Collision)
        {
          NewLink->Collisions.Add(Collision);
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Creation of Virtual Link %s failed"), *CurrentLinkName);
        }
    }

  // Add link to the data asset
  OutModel->Links.Add(NewLink);
}

// Parse <visual> node
void FURDFParser::ParseVisual(const FXmlNode* InNode, USDFLink*& OutLink) {
  USDFVisual* NewVisual = nullptr;

  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty()) {
    NewVisual = NewObject<USDFVisual>(OutLink, FName(*Name));
    NewVisual->Name = Name;
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> has no \"name\" attribute, added a default value.."), *FString(__FUNCTION__), __LINE__);
    NewVisual = NewObject<USDFVisual>(OutLink/*, FName(TEXT("__default__"))*/);
    NewVisual->Name = TEXT("__default__");
  }

  // Iterate <visual> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes()) {
    if (ChildNode->GetTag().Equals(TEXT("origin"))) {
      // TODO
      NewVisual->Pose = PoseContentToFTransform(ChildNode->GetAttribute(TEXT("xyz")) + " " + ChildNode->GetAttribute(TEXT("rpy")));
    } else if (ChildNode->GetTag().Equals(TEXT("geometry"))) {
      ParseGeometry(ChildNode, NewVisual->Geometry, ESDFType::Visual);
      if(NewVisual->Geometry->Type == ESDFGeometryType::Box ||
          NewVisual->Geometry->Type == ESDFGeometryType::Cylinder ||
          NewVisual->Geometry->Type == ESDFGeometryType::Sphere) {
        NewVisual->Geometry->Mesh = CreateMesh(ESDFType::Visual, NewVisual->Geometry->Type, Name, RStaticMeshUtils::GetGeometryParameter(NewVisual->Geometry));
      }
    } else {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> <visual> child <%s> not supported, ignored.."), *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
      continue;
    }
  }
  OutLink->Visuals.Add(NewVisual);
}

// Parse <collision> node
void FURDFParser::ParseCollision(const FXmlNode* InNode, USDFLink*& OutLink) {
  USDFCollision* NewCollision = nullptr;

  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty()) {
    NewCollision = NewObject<USDFCollision>(OutLink, FName(*Name));
    NewCollision->Name = Name;
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <collision> has no \"name\" attribute, added a default value.."), *FString(__FUNCTION__), __LINE__);
    NewCollision = NewObject<USDFCollision>(OutLink/*, FName(TEXT("__default__"))*/);
    NewCollision->Name = TEXT("__default__");
  }

  // Iterate <collision> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes()) {
    if (ChildNode->GetTag().Equals(TEXT("origin"))) {
      // TODO
      NewCollision->Pose = PoseContentToFTransform(ChildNode->GetAttribute(TEXT("xyz")) + " " + ChildNode->GetAttribute(TEXT("rpy")));
    } else if (ChildNode->GetTag().Equals(TEXT("geometry"))) {
      ParseGeometry(ChildNode, NewCollision->Geometry, ESDFType::Collision);
      if(NewCollision->Geometry->Type == ESDFGeometryType::Box ||
          NewCollision->Geometry->Type == ESDFGeometryType::Cylinder ||
          NewCollision->Geometry->Type == ESDFGeometryType::Sphere) {
        NewCollision->Geometry->Mesh = CreateMesh(ESDFType::Collision, NewCollision->Geometry->Type, Name, RStaticMeshUtils::GetGeometryParameter(NewCollision->Geometry));
        // RStaticMeshUtils::CreateStaticMeshThroughBrush(OutLink,NewCollision);
      }
    } else {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <inertial> child <%s> not supported, ignored.."), *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
      continue;
    }
  }

  // Add collision to array
  OutLink->Collisions.Add(NewCollision);
}

// Parse <geometry> <mesh> node
void FURDFParser::ParseGeometryMesh(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type) {
  OutGeometry->Type = ESDFGeometryType::Mesh;

  if (!InNode->GetAttribute(TEXT("filename")).IsEmpty()) {
    OutGeometry->Uri = InNode->GetAttribute(TEXT("filename"));
    OutGeometry->Mesh = ImportMesh(OutGeometry->Uri, Type);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <mesh> is missing filename, ignored.."), *FString(__FUNCTION__), __LINE__);
  }
}

void FURDFParser::ParseLinkInertial(const FXmlNode* InNode, USDFLink*& OutLink) {
  // Iterate <link> <inertial> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes()) {
    if (ChildNode->GetTag().Equals(TEXT("mass"))) {
      OutLink->Inertial->Mass = FCString::Atof(*ChildNode->GetContent());
    } else if (ChildNode->GetTag().Equals(TEXT("origin"))) {
      // TODO
      OutLink->Inertial->Pose = PoseContentToFTransform(ChildNode->GetAttribute(TEXT("xyz")) + " " + ChildNode->GetAttribute(TEXT("rpy")));
    } else {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <inertial> child <%s> not supported, ignored.."), *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
      continue;
    }
  }
}

// Parse <geometry> node
void FURDFParser::ParseGeometry(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type) {
  // Iterate <geometry> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes()) {
    if (ChildNode->GetTag().Equals(TEXT("mesh"))) {
      ParseGeometryMesh(ChildNode, OutGeometry, Type);
    } else if (ChildNode->GetTag().Equals(TEXT("box"))) {
      ParseGeometryBox(ChildNode, OutGeometry);
    } else if (ChildNode->GetTag().Equals(TEXT("cylinder"))) {
      ParseGeometryCylinder(ChildNode, OutGeometry);
    } else if (ChildNode->GetTag().Equals(TEXT("sphere"))) {
      ParseGeometrySphere(ChildNode, OutGeometry);
    } else {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> child <%s> not supported, ignored.."), *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
      continue;
    }
  }
}

// Parse <geometry> <box> node
void FURDFParser::ParseGeometryBox(const FXmlNode* InNode, USDFGeometry*& OutGeometry) {
  OutGeometry->Type = ESDFGeometryType::Box;

  if (!InNode->GetAttribute(TEXT("size")).IsEmpty()) {
    OutGeometry->Size = SizeToFVector(InNode->GetAttribute(TEXT("size")));
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <box> has no size attribute, ignored.."), *FString(__FUNCTION__), __LINE__);
  }
}

// Parse <geometry> <cylinder> node
void FURDFParser::ParseGeometryCylinder(const FXmlNode* InNode, USDFGeometry*& OutGeometry) {
  OutGeometry->Type = ESDFGeometryType::Cylinder;

  if (!InNode->GetAttribute(TEXT("radius")).IsEmpty()) {
    OutGeometry->Radius = FConversions::MToCm(FCString::Atof(*InNode->GetAttribute(TEXT("radius"))));
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <box> has no radius attribute, ignored.."), *FString(__FUNCTION__), __LINE__);
  }

    if (!InNode->GetAttribute(TEXT("length")).IsEmpty()) {
     OutGeometry->Length = FConversions::MToCm(FCString::Atof(*InNode->GetAttribute(TEXT("length"))));
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <box> has no length attribute, ignored.."), *FString(__FUNCTION__), __LINE__);
  }
}

// Parse <geometry> <sphere> node
void FURDFParser::ParseGeometrySphere(const FXmlNode* InNode, USDFGeometry*& OutGeometry) {
  OutGeometry->Type = ESDFGeometryType::Sphere;

  if (!InNode->GetAttribute(TEXT("radius")).IsEmpty()) {
    OutGeometry->Radius = FConversions::MToCm(FCString::Atof(*InNode->GetAttribute(TEXT("radius"))));
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <box> has no radius attribute, ignored.."), *FString(__FUNCTION__), __LINE__);
  }
}

// Parse <joint> node
void FURDFParser::ParseJoint(const FXmlNode* InNode, USDFModel*& OutModel) {
  // Pointer to the new joint
  USDFJoint* NewJoint = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty()) {
    NewJoint = NewObject<USDFJoint>(OutModel, FName(*Name));
    NewJoint->Name = Name;
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> has no \"name\" attribute, added a default value.."), *FString(__FUNCTION__), __LINE__);
    NewJoint = NewObject<USDFJoint>(OutModel/*, FName(TEXT("__default__"))*/);
    NewJoint->Name = TEXT("__default__");
  }

  // Get "type" from node attribute
  const FString Type = InNode->GetAttribute(TEXT("type"));
  if (!Type.IsEmpty()) {
    NewJoint->Type = Type;
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> has no \"type\" attribute, added a default value.."), *FString(__FUNCTION__), __LINE__);
    NewJoint->Name = TEXT("__default__");
  }

  NewJoint->Parent = InNode->FindChildNode(TEXT("parent"))->GetAttribute(TEXT("link"));
  NewJoint->Child = InNode->FindChildNode(TEXT("Child"))->GetAttribute(TEXT("link"));
  // Iterate <joint> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes()) {
    if (ChildNode->GetTag().Equals(TEXT("parent")) || ChildNode->GetTag().Equals(TEXT("child"))) {
    } else if (ChildNode->GetTag().Equals(TEXT("origin"))) {
      NewJoint->Pose = PoseContentToFTransform(ChildNode->GetAttribute(TEXT("xyz")) + " " + ChildNode->GetAttribute(TEXT("rpy")));
      FTransform Relative = FindRelativeTransform(NewJoint->Child, OutModel);
      FTransform::Multiply(&NewJoint->Pose, &NewJoint->Pose, &Relative);
    }
    else if (ChildNode->GetTag().Equals(TEXT("limit"))) {
      ParseJointAxisLimit(ChildNode, NewJoint);
    } else if (ChildNode->GetTag().Equals(TEXT("axis"))) {
      ParseJointAxis(ChildNode, NewJoint);
    } else {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> child <%s> not supported, ignored.."), *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
      continue;
    }
  }

  // Add link to the data asset
  OutModel->Joints.Add(NewJoint);
}

// Parse <joint> <axis> node
void FURDFParser::ParseJointAxis(const FXmlNode* InNode, USDFJoint*& OutJoint) {
  OutJoint->Axis->Xyz = XyzToFVector(InNode->GetAttribute(TEXT("xyz")));
  OutJoint->Axis->bUseParentModelFrame = true;
}

// Parse <joint> <axis> <limit> node
void FURDFParser::ParseJointAxisLimit(const FXmlNode* InNode, USDFJoint*& OutJoint) {
  if (!InNode->GetAttribute(TEXT("lower")).IsEmpty()) {
    OutJoint->Axis->SetLowerLimitFromSDF(FCString::Atof(*InNode->GetAttribute(TEXT("lower"))));
  }
  if (!InNode->GetAttribute(TEXT("upper")).IsEmpty()) {
    OutJoint->Axis->SetUpperLimitFromSDF(FCString::Atof(*InNode->GetAttribute(TEXT("upper"))));
  }
  if (!InNode->GetAttribute(TEXT("effort")).IsEmpty()) {
    OutJoint->Axis->Effort = FCString::Atof(*InNode->GetAttribute(TEXT("effort")));
  }
  if (!InNode->GetAttribute(TEXT("velocity")).IsEmpty()) {
    OutJoint->Axis->Velocity = FCString::Atof(*InNode->GetAttribute(TEXT("velocity")));
  }
}


/* Begin helper functions */
// Fix file path
void FURDFParser::SetDirectoryPath(const FString& InFilename)
{
  // Replace back slash to forward slash in the path
  // D:\Models\MyModelName\model.sdf --> D:/Models/MyModelName/model.sdf
  DirPath = InFilename.Replace(TEXT("\\"), TEXT("/"));

  // Remove filename from path --> D:/Models/MyModelName
  DirPath = FPaths::GetPath(DirPath);

  // TODO rather cut uppermost from URI
  // One dir up --> D:/Models
  DirPath = FPaths::Combine(DirPath, TEXT("/../"));
  FPaths::CollapseRelativeDirectories(DirPath);
}

// Get mesh absolute path
FString FURDFParser::GetMeshAbsolutePath(const FString& Uri)
{
  // Create mesh relative path, add .fbx extension
  FString MeshRelativePath = Uri;
  if (!MeshRelativePath.EndsWith(".fbx"))
    {
      MeshRelativePath = FPaths::ChangeExtension(MeshRelativePath, TEXT("fbx"));
    }
  // Remove package name prefix
  MeshRelativePath.RemoveFromStart(TEXT("model://"));
  TArray<FString> PackageParts;
  MeshRelativePath.ParseIntoArray(PackageParts, TEXT("/"));
  FString PackageName = PackageParts[0];


  FString PackagePath = GetROSPackagePath(PackageName);

  if(PackagePath.IsEmpty())
    {
      // Create mesh absolute path
      return DirPath + MeshRelativePath;
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("MeshPath %s"), *(PackagePath + MeshRelativePath));
      return PackagePath + MeshRelativePath;
    }
}

FName FURDFParser::GenerateMeshName(ESDFType InType, FString InName)
{
  FName MeshName;
  if (InType == ESDFType::Collision)
    {
      MeshName = FName(*(TEXT("SM_") + InName + TEXT("_C")));
    }
  else if (InType == ESDFType::Visual)
    {
      MeshName = FName(*(TEXT("SM_") + InName + TEXT("_V")));
    }
  return MeshName;
}

FString FURDFParser::GeneratePackageName(FName MeshName)
{
  FString PackageName = "";
  FString Reason = "";
  FString NewDir = DataAsset->GetOuter()->GetPathName() + "/" + CurrentLinkName;
  if(!FPackageName::TryConvertFilenameToLongPackageName(NewDir + "/" + MeshName.ToString() , PackageName, &Reason))
    {
      UE_LOG(LogTemp, Error, TEXT("Packacke name invlaide because : %s"), *Reason);
    }
  AssetRegistryModule.Get().AddPath(NewDir);
  return PackageName;
}

UStaticMesh* FURDFParser::CreateMesh(ESDFType InType, ESDFGeometryType InShape, FString InName, TArray<float> InParameters)
{
  // FString Path = "";
  FName MeshName = GenerateMeshName(InType, InName);
  FString PackageName = GeneratePackageName(MeshName);


  //UPackage* Pkg = CreatePackage(NULL, *PackageName);
  UPackage* Pkg = CreatePackage(*PackageName);
  UStaticMesh* Mesh = RStaticMeshUtils::CreateStaticMesh(Pkg, PackageName, InShape, InParameters);
  CreateCollisionForMesh(Mesh, InShape);
  return Mesh;
}

void FURDFParser::GetROSPackagePaths()
{
  FString TempPath = FPlatformMisc::GetEnvironmentVariable(TEXT("ROS_PACKAGE_PATH"));
  if (!TempPath.IsEmpty())
    {
      ROSPackagePaths.Empty();
      TArray<FString> TempPathArray;
      TempPath.ParseIntoArray(TempPathArray, TEXT(":"));
      for(auto & Path : TempPathArray)
        {
          TArray<FString> TempPackageParts;
          Path.ParseIntoArray(TempPackageParts, TEXT("/"));
          FString PackageName = TempPackageParts.Top();
          if(!Path.Contains("opt"))
            {
              Path.RemoveFromEnd(PackageName);
              ROSPackagePaths.Add(PackageName, Path);
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("ROS default path %s"), *Path);
              ROSPackagePaths.Add(TEXT("ROS"), Path + TEXT("/"));
            }
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("ROS_PACKAGE_PATH is empty or not set"));
    }
}

FString FURDFParser::GetROSPackagePath(const FString& InPackageName)
{
  if(ROSPackagePaths.Contains(InPackageName))
    {
      return ROSPackagePaths[InPackageName];
    }
  else
    {
      if(ROSPackagePaths.Contains(TEXT("ROS")))
        {
          FString TestPath = FPaths::Combine(ROSPackagePaths[TEXT("ROS")],  InPackageName);
          if(FPaths::DirectoryExists(TestPath))
            {
              return ROSPackagePaths[TEXT("ROS")];
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("[%s] testpath %s does not exist"), *FString(__FUNCTION__), *InPackageName);
            }
        }
      UE_LOG(LogTemp, Error, TEXT("[%s] ROSPackage %s not found"), *FString(__FUNCTION__), *InPackageName);
      return FString();
    }
}

// Import .fbx meshes from data asset
UStaticMesh* FURDFParser::ImportMesh(const FString& Uri, ESDFType Type)
{
  FString MeshAbsolutePath = GetMeshAbsolutePath(Uri);
  if (!FPaths::FileExists(MeshAbsolutePath))
    {
      UE_LOG(LogTemp, Error, TEXT("[%s] Could not find %s"), *FString(__FUNCTION__), *MeshAbsolutePath);
      return nullptr;
    }

  // Get mesh name

  FString MeshNameTemp = FPaths::GetBaseFilename(MeshAbsolutePath);
  FName MeshName = GenerateMeshName(Type, MeshNameTemp);

  // Import mesh
  bool bOperationCancelled = false;

  FString PackageName = GeneratePackageName(MeshName);
  //UPackage* Pkg = CreatePackage(NULL, *PackageName);
  UPackage* Pkg = CreatePackage(*PackageName);
  UObject* MeshObj = FbxFactory->ImportObject(
                                              UStaticMesh::StaticClass(), Pkg, MeshName, RF_Transactional | RF_Standalone | RF_Public, MeshAbsolutePath, nullptr, bOperationCancelled);

  // If import has been cancelled
  if (bOperationCancelled)
    {
      return nullptr;
    }

  FAssetRegistryModule::AssetCreated(MeshObj);

  // Mark outer dirty
  if (DataAsset->GetOuter())
    {
      DataAsset->GetOuter()->MarkPackageDirty();
    }
  else
    {
      DataAsset->MarkPackageDirty();
    }
  // Mark mesh dirty
  if (MeshObj)
    {
      MeshObj->MarkPackageDirty();
    }

  // Return the cast result
  return Cast<UStaticMesh>(MeshObj);
}


bool FURDFParser::CreateCollisionForMesh(UStaticMesh* OutMesh, ESDFGeometryType Type)
{
  switch(Type)
    {
    case ESDFGeometryType::None :
      return false;
    case ESDFGeometryType::Mesh :
      return true;
    case ESDFGeometryType::Box :
      RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopX10);
      return true;
    case ESDFGeometryType::Cylinder :
      RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopZ10);
      return true;
    case ESDFGeometryType::Sphere :
      RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopX10);
      return true;
    default :
      UE_LOG(LogTemp, Error, TEXT("GeometryType not supportet for %s."), *OutMesh->GetName());
      return false;
    }
}

USDFCollision* FURDFParser::CreateVirtualCollision(USDFLink* OutLink)
{
  USDFCollision* NewCollision = NewObject<USDFCollision>(OutLink, FName(*CurrentLinkName));
  NewCollision->Name = CurrentLinkName;
  NewCollision->Pose = FTransform();
  NewCollision->Geometry = NewObject<USDFGeometry>(NewCollision);
  NewCollision->Geometry->Type = ESDFGeometryType::Box;
  NewCollision->Geometry->Size = FVector(0.5f, 0.5f, 0.5f);
  NewCollision->Geometry->Mesh = CreateMesh(ESDFType::Collision, ESDFGeometryType::Box, CurrentLinkName, RStaticMeshUtils::GetGeometryParameter(NewCollision->Geometry));
  return NewCollision;
}
