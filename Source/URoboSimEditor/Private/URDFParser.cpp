#include "URDFParser.h"
#include "Conversions.h"
// #include "Paths.h"
#include "XmlFile.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Factories/FbxFactory.h"
#include "RStaticMeshEditUtils.h"

// Default constructor
FURDFParser::FURDFParser()
{
    this->XmlFile=nullptr;
    this->bLoaded=false;
    GetROSPackagePaths();
}

// Constructor with load from path
FURDFParser::FURDFParser(const FString& InFilename)
{
  this->XmlFile=nullptr;
  this->bLoaded=false;
  GetROSPackagePaths();
  Load(InFilename);
}

// Destructor
FURDFParser::~FURDFParser()
{
  Clear();
}

// Load urdf from file
bool FURDFParser::Load(const FString& InFilename)
{
  AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
  // Make sure parser is clean
  Clear();

  XmlFile = new FXmlFile(InFilename);
  bLoaded = IsValid();
  SetDirectoryPath(InFilename);
  FbxFactory = NewObject<UFbxFactory>(UFbxFactory::StaticClass());
  FbxFactory->EnableShowOption();

  return bLoaded;
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
  if (bLoaded)
    {
      bLoaded = false;
      DataAsset = nullptr;
      FbxFactory = nullptr;
      DirPath = TEXT("");
    }
}

// Create data asset and parse urdf data into it
URDDataAsset* FURDFParser::ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags)
{
  if (!bLoaded)
    {
      return nullptr;
    }

  // Create a new RDDataAsset
  DataAsset = NewObject<URDDataAsset>(InParent, InName, InFlags);

  // Parse urdf data and fill the data asset
  Parse();

  return DataAsset;
}

// Check if urdf data is valid
bool FURDFParser::IsValid() {
  if (XmlFile == nullptr) {
    return false;
  }

  // Check if root node is <urdf>  (urdf version 1.2)
  if (!XmlFile->GetRootNode()->GetTag().Equals(TEXT("robot"))) {
    UE_LOG(LogTemp, Error, TEXT("[%s][%d] Root node is not <robot>"), TEXT(__FUNCTION__), __LINE__);
    return false;
  }
  return true;
}

// Parse <robot> node
void FURDFParser::Parse() {
  DataAsset->Version = TEXT("__default__");

  URDModel* NewModel = nullptr;

  // Get "name" from node attribute
  const FString Name = XmlFile->GetRootNode()->GetAttribute(TEXT("name"));
  if(!Name.IsEmpty()) {
    NewModel = NewObject<URDModel>(DataAsset, FName(*Name));
    NewModel->Name = Name;
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <model> has no \"name\" attribute, added a default value.."), *FString(__FUNCTION__), __LINE__);
    NewModel = NewObject<URDModel>(DataAsset/*, FName(TEXT("__default__"))*/);
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

  for (URDJoint* Joint : NewModel->Joints) {
    FTransform Relative = FindRelativeTransform(Joint->Parent, NewModel);
    URDLink* Link = FindLink(Joint->Child, NewModel);
    FTransform Transform = Joint->Pose;
    Joint->Axis->bUseParentModelFrame = false;
    FTransform::Multiply(&Link->Pose, &Transform, &Relative);
  }

  DataAsset->Models.Add(NewModel);
}

// Parse <link> node
void FURDFParser::ParseLink(const FXmlNode* InNode, URDModel*& OutModel) {
  // Ptr to the new link
  URDLink* NewLink = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty()) {
    NewLink = NewObject<URDLink>(OutModel, FName(*Name));
    NewLink->Name = Name;
    CurrentLinkName = Name;
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> has no \"name\" attribute, added a default value.."), *FString(__FUNCTION__), __LINE__);
    NewLink = NewObject<URDLink>(OutModel/*, FName(TEXT("__default__"))*/);
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
      URDCollision* Collision = CreateVirtualCollision(NewLink);
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
void FURDFParser::ParseVisual(const FXmlNode* InNode, URDLink*& OutLink) {
  URDVisual* NewVisual = nullptr;

  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty()) {
    NewVisual = NewObject<URDVisual>(OutLink, FName(*Name));
    NewVisual->Name = Name;
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> has no \"name\" attribute, added a default value.."), *FString(__FUNCTION__), __LINE__);
    NewVisual = NewObject<URDVisual>(OutLink/*, FName(TEXT("__default__"))*/);
    NewVisual->Name = OutLink->Name + TEXT("_visual");
  }

  // Iterate <visual> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes()) {
    if (ChildNode->GetTag().Equals(TEXT("origin"))) {
      // TODO
      NewVisual->Pose = PoseContentToFTransform(ChildNode->GetAttribute(TEXT("xyz")) + " " + ChildNode->GetAttribute(TEXT("rpy")));
    } else if (ChildNode->GetTag().Equals(TEXT("geometry"))) {
      ParseGeometry(ChildNode, NewVisual->Geometry, ERDType::Visual);
      if(NewVisual->Geometry->Type == ERDGeometryType::Box ||
          NewVisual->Geometry->Type == ERDGeometryType::Cylinder ||
          NewVisual->Geometry->Type == ERDGeometryType::Sphere) {
        NewVisual->Geometry->Mesh = CreateMesh(ERDType::Visual, NewVisual->Geometry->Type, Name, RStaticMeshUtils::GetGeometryParameter(NewVisual->Geometry));
      }
    } else {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> <visual> child <%s> not supported, ignored.."), *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
      continue;
    }
  }
  OutLink->Visuals.Add(NewVisual);
}

// Parse <collision> node
void FURDFParser::ParseCollision(const FXmlNode* InNode, URDLink*& OutLink) {
  URDCollision* NewCollision = nullptr;

  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty()) {
    NewCollision = NewObject<URDCollision>(OutLink, FName(*Name));
    NewCollision->Name = Name;
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <collision> has no \"name\" attribute, added a default value.."), *FString(__FUNCTION__), __LINE__);
    NewCollision = NewObject<URDCollision>(OutLink/*, FName(TEXT("__default__"))*/);
    NewCollision->Name = OutLink->Name + TEXT("_collision");
  }

  // Iterate <collision> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes()) {
    if (ChildNode->GetTag().Equals(TEXT("origin"))) {
      // TODO
      NewCollision->Pose = PoseContentToFTransform(ChildNode->GetAttribute(TEXT("xyz")) + " " + ChildNode->GetAttribute(TEXT("rpy")));
    } else if (ChildNode->GetTag().Equals(TEXT("geometry"))) {
      ParseGeometry(ChildNode, NewCollision->Geometry, ERDType::Collision);
      if(NewCollision->Geometry->Type == ERDGeometryType::Box ||
          NewCollision->Geometry->Type == ERDGeometryType::Cylinder ||
          NewCollision->Geometry->Type == ERDGeometryType::Sphere) {
        NewCollision->Geometry->Mesh = CreateMesh(ERDType::Collision, NewCollision->Geometry->Type, Name, RStaticMeshUtils::GetGeometryParameter(NewCollision->Geometry));
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
void FURDFParser::ParseGeometryMesh(const FXmlNode* InNode, URDGeometry*& OutGeometry, ERDType Type) {
  OutGeometry->Type = ERDGeometryType::Mesh;

  if (!InNode->GetAttribute(TEXT("filename")).IsEmpty()) {
    OutGeometry->Uri = InNode->GetAttribute(TEXT("filename"));
    OutGeometry->Mesh = ImportMesh(OutGeometry->Uri, Type);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <mesh> is missing filename, ignored.."), *FString(__FUNCTION__), __LINE__);
  }
}

void FURDFParser::ParseLinkInertial(const FXmlNode* InNode, URDLink*& OutLink) {
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
void FURDFParser::ParseGeometry(const FXmlNode* InNode, URDGeometry*& OutGeometry, ERDType Type) {
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
void FURDFParser::ParseGeometryBox(const FXmlNode* InNode, URDGeometry*& OutGeometry) {
  OutGeometry->Type = ERDGeometryType::Box;

  if (!InNode->GetAttribute(TEXT("size")).IsEmpty()) {
    OutGeometry->Size = SizeToFVector(InNode->GetAttribute(TEXT("size")));
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <box> has no size attribute, ignored.."), *FString(__FUNCTION__), __LINE__);
  }
}

// Parse <geometry> <cylinder> node
void FURDFParser::ParseGeometryCylinder(const FXmlNode* InNode, URDGeometry*& OutGeometry) {
  OutGeometry->Type = ERDGeometryType::Cylinder;

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
void FURDFParser::ParseGeometrySphere(const FXmlNode* InNode, URDGeometry*& OutGeometry) {
  OutGeometry->Type = ERDGeometryType::Sphere;

  if (!InNode->GetAttribute(TEXT("radius")).IsEmpty()) {
    OutGeometry->Radius = FConversions::MToCm(FCString::Atof(*InNode->GetAttribute(TEXT("radius"))));
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <box> has no radius attribute, ignored.."), *FString(__FUNCTION__), __LINE__);
  }
}

// Parse <joint> node
void FURDFParser::ParseJoint(const FXmlNode* InNode, URDModel*& OutModel) {
  // Pointer to the new joint
  URDJoint* NewJoint = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty()) {
    NewJoint = NewObject<URDJoint>(OutModel, FName(*Name));
    NewJoint->Name = Name;
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> has no \"name\" attribute, added a default value.."), *FString(__FUNCTION__), __LINE__);
    NewJoint = NewObject<URDJoint>(OutModel/*, FName(TEXT("__default__"))*/);
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
  NewJoint->Child = InNode->FindChildNode(TEXT("child"))->GetAttribute(TEXT("link"));
  // Iterate <joint> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes()) {
    if (ChildNode->GetTag().Equals(TEXT("parent")) || ChildNode->GetTag().Equals(TEXT("child"))) {
    } else if (ChildNode->GetTag().Equals(TEXT("origin"))) {
      NewJoint->Pose = PoseContentToFTransform(ChildNode->GetAttribute(TEXT("xyz")) + " " + ChildNode->GetAttribute(TEXT("rpy")));
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
void FURDFParser::ParseJointAxis(const FXmlNode* InNode, URDJoint*& OutJoint) {
  OutJoint->Axis->Xyz = XyzToFVector(InNode->GetAttribute(TEXT("xyz")));
  OutJoint->Axis->bUseParentModelFrame = true;
}

// Parse <joint> <axis> <limit> node
void FURDFParser::ParseJointAxisLimit(const FXmlNode* InNode, URDJoint*& OutJoint) {
  if (!InNode->GetAttribute(TEXT("lower")).IsEmpty()) {
    OutJoint->Axis->SetLowerLimitFrom(FCString::Atof(*InNode->GetAttribute(TEXT("lower"))));
  }
  if (!InNode->GetAttribute(TEXT("upper")).IsEmpty()) {
    OutJoint->Axis->SetUpperLimitFrom(FCString::Atof(*InNode->GetAttribute(TEXT("upper"))));
  }
  if (!InNode->GetAttribute(TEXT("effort")).IsEmpty()) {
    OutJoint->Axis->Effort = FCString::Atof(*InNode->GetAttribute(TEXT("effort")));
  }
  if (!InNode->GetAttribute(TEXT("velocity")).IsEmpty()) {
    OutJoint->Axis->Velocity = FCString::Atof(*InNode->GetAttribute(TEXT("velocity")));
  }
}
