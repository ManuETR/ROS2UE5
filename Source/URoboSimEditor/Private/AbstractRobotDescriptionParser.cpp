#include "AbstractRobotDescriptionParser.h"
#include "XmlFile.h"
#include "Conversions.h"

FAbstractRobotDescriptionParser::FAbstractRobotDescriptionParser() :  AssetRegistryModule(FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"))){}

// Find the FTransform from the relative to Element
FTransform FAbstractRobotDescriptionParser::FindRelativeTransform(const FString RelativeTo, URDModel* Model) {
  int32 IndexJoint = Model->Joints.FindLastByPredicate([RelativeTo](const URDJoint* Joint) { return Joint->Name == RelativeTo; });
  int32 IndexLink = Model->Links.FindLastByPredicate([RelativeTo](const URDLink* Link) { return Link->Name == RelativeTo; });

  if (IndexJoint != INDEX_NONE) {
    return Model->Joints[IndexJoint]->Pose;
  }
  else if (IndexLink != INDEX_NONE) {
    return Model->Links[IndexLink]->Pose;
  }
  else {
    UE_LOG(LogTemp, Error, TEXT("Relative transformation %s not found."), FString(RelativeTo));
    return FTransform();
  }
}

URDLink* FAbstractRobotDescriptionParser::FindLink(const FString Needle, URDModel* Model) {
  int32 Index = Model->Links.FindLastByPredicate([Needle](const URDLink* Link) { return Link->Name == Needle; });

  if (Index != INDEX_NONE) {
    return Model->Links[Index];
  }
  else {
    UE_LOG(LogTemp, Error, TEXT("Link with the name %s cannot be found."), *FString(Needle));
    return nullptr;
  }
}

URDJoint* FAbstractRobotDescriptionParser::FindJoint(const FString Needle, URDModel* Model) {
  int32 Index = Model->Joints.FindLastByPredicate([Needle](const URDJoint* Joint) { return Joint->Name == Needle; });

  if (Index != INDEX_NONE) {
    return Model->Joints[Index];
  }
  else {
    UE_LOG(LogTemp, Error, TEXT("Joint with the name %s cannot be found."), *FString(Needle));
    return nullptr;
  }
}



// From <pose>z y z r p y</pose> to FTransform
FTransform FAbstractRobotDescriptionParser::PoseContentToFTransform(const FString& InPoseData)
{
  // <pose>x=[0] y=[1] z=[2] r=[3] p=[4] y=[5]</pose>
  TArray<FString> PoseDataArray;
  int32 ArrSize = InPoseData.ParseIntoArray(PoseDataArray, TEXT(" "), true);

  if (ArrSize == 6)
    {
      const FRotator Rot(FConversions::ROSToU(FCString::Atof(*PoseDataArray[3]) , FCString::Atof(*PoseDataArray[4]), FCString::Atof(*PoseDataArray[5])));		// roll

      const FVector Loc = FVector(FCString::Atof(*PoseDataArray[0]),
                                  FCString::Atof(*PoseDataArray[1]),
                                  FCString::Atof(*PoseDataArray[2]));

      const FTransform Trans = FTransform(Rot, FConversions::ROSToU(Loc));

      return Trans;
    }

  UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <pose>%s</pose> is an unsupported format!"),
         *FString(__FUNCTION__), __LINE__, *InPoseData);
  return FTransform();
}

// From <>x y z</> to FVector
FVector FAbstractRobotDescriptionParser::XyzToFVector(const FString& InXyzData)
{
  // <size>x=[0] y=[1] z=[2]</size>
  TArray<FString> DataArray;
  int32 ArrSize = InXyzData.ParseIntoArray(DataArray, TEXT(" "), true);

  if (ArrSize == 3)
    {
      return FVector(FCString::Atof(*DataArray[0]),
                     -FCString::Atof(*DataArray[1]),
                     FCString::Atof(*DataArray[2]));
    }

  // Unsupported <pose> format, return default transform
  UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <xyz>%s</xyz> is an unsupported format!"),
         __FUNCTION__, __LINE__, *InXyzData);
  return FVector();
}

// From <size>z y z</size> to FVector
FVector FAbstractRobotDescriptionParser::SizeToFVector(const FString& InSizeData)
{
  // <size>x=[0] y=[1] z=[2]</size>
  TArray<FString> DataArray;
  int32 ArrSize = InSizeData.ParseIntoArray(DataArray, TEXT(" "), true);

  if (ArrSize == 3)
    {
      return FConversions::MToCm(FVector(FCString::Atof(*DataArray[0]),
                                         FCString::Atof(*DataArray[1]),
                                         FCString::Atof(*DataArray[2])));
    }

  // Unsupported <pose> format, return default transform
  UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <size>%s</size> is an unsupported format!"),
         __FUNCTION__, __LINE__, *InSizeData);
  return FVector();
}

// Fix file path
void FAbstractRobotDescriptionParser::SetDirectoryPath(const FString& InFilename)
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
FString FAbstractRobotDescriptionParser::GetMeshAbsolutePath(const FString& Uri)
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

FName FAbstractRobotDescriptionParser::GenerateMeshName(ERDType InType, FString InName)
{
  FName MeshName;
  if (InType == ERDType::Collision)
    {
      MeshName = FName(*(TEXT("SM_") + InName + TEXT("_C")));
    }
  else if (InType == ERDType::Visual)
    {
      MeshName = FName(*(TEXT("SM_") + InName + TEXT("_V")));
    }
  return MeshName;
}

FString FAbstractRobotDescriptionParser::GeneratePackageName(FName MeshName)
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

UStaticMesh* FAbstractRobotDescriptionParser::CreateMesh(ERDType InType, ERDGeometryType InShape, FString InName, TArray<float> InParameters)
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

void FAbstractRobotDescriptionParser::GetROSPackagePaths()
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

FString FAbstractRobotDescriptionParser::GetROSPackagePath(const FString& InPackageName)
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
UStaticMesh* FAbstractRobotDescriptionParser::ImportMesh(const FString& Uri, ERDType Type)
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


bool FAbstractRobotDescriptionParser::CreateCollisionForMesh(UStaticMesh* OutMesh, ERDGeometryType Type)
{
  switch(Type)
    {
    case ERDGeometryType::None :
      return false;
    case ERDGeometryType::Mesh :
      return true;
    case ERDGeometryType::Box :
      RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopX10);
      return true;
    case ERDGeometryType::Cylinder :
      RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopZ10);
      return true;
    case ERDGeometryType::Sphere :
      RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopX10);
      return true;
    default :
      UE_LOG(LogTemp, Error, TEXT("GeometryType not supportet for %s."), *OutMesh->GetName());
      return false;
    }
}

URDCollision* FAbstractRobotDescriptionParser::CreateVirtualCollision(URDLink* OutLink)
{
  URDCollision* NewCollision = NewObject<URDCollision>(OutLink, FName(*CurrentLinkName));
  NewCollision->Name = CurrentLinkName;
  NewCollision->Pose = FTransform();
  NewCollision->Geometry = NewObject<URDGeometry>(NewCollision);
  NewCollision->Geometry->Type = ERDGeometryType::Box;
  NewCollision->Geometry->Size = FVector(0.5f, 0.5f, 0.5f);
  NewCollision->Geometry->Mesh = CreateMesh(ERDType::Collision, ERDGeometryType::Box, CurrentLinkName, RStaticMeshUtils::GetGeometryParameter(NewCollision->Geometry));
  return NewCollision;
}
