#include "SDFParser.h"
#include "Conversions.h"
// #include "Paths.h"
#include "XmlFile.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Factories/FbxFactory.h"
#include "RStaticMeshEditUtils.h"

// Default constructor
FSDFParser::FSDFParser()
{
    this->XmlFile=nullptr;
    this->bLoaded=false;
    GetROSPackagePaths();
}

// Constructor with load from path
FSDFParser::FSDFParser(const FString& InFilename)
{
  this->XmlFile=nullptr;
  this->bLoaded=false;
  GetROSPackagePaths();
  Load(InFilename);
}

// Destructor
FSDFParser::~FSDFParser()
{
  Clear();
}

// Load sdf from file
bool FSDFParser::Load(const FString& InFilename)
{
  AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
  // Make sure parser is clean
  Clear();

  // Load xml file
  XmlFile = new FXmlFile(InFilename);

  // Check for valid sdf
  bLoaded = IsValid();

  // Set the model directory path
  SetDirectoryPath(InFilename);

  // Create fbx factory for loading the meshes
  FbxFactory = NewObject<UFbxFactory>(UFbxFactory::StaticClass());

  // If options are to be offered before loading.
  FbxFactory->EnableShowOption();

  return bLoaded;
}

// Clear parser
void FSDFParser::Clear()
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

// Create data asset and parse sdf data into it
USDFDataAsset* FSDFParser::ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags)
{
  if (!bLoaded)
    {
      return nullptr;
    }

  // Create a new SDFDataAsset
  DataAsset = NewObject<USDFDataAsset>(InParent, InName, InFlags);

  // Parse sdf data and fill the data asset
  Parse();

  return DataAsset;
}

// Check if sdf data is valid
bool FSDFParser::IsValid()
{
  if (XmlFile == nullptr)
    {
      return false;
    }

  // Check if root node is <sdf> or <gazebo> (sdf version 1.2)
  if (!XmlFile->GetRootNode()->GetTag().Equals(TEXT("sdf"))
      && !XmlFile->GetRootNode()->GetTag().Equals(TEXT("gazebo")))
    {
      // UE_LOG(LogTemp, Error, TEXT("[%s][%d] Root node is not <sdf> or <gazebo>(sdf version 1.2)"), TEXT(__FUNCTION__), __LINE__);
      return false;
    }
  return true;
}

// Parse <sdf> node
void FSDFParser::Parse()
{
  // Get "version" from node attribute
  const FString SDFVersion = XmlFile->GetRootNode()->GetAttribute(TEXT("version"));
  if (!SDFVersion.IsEmpty())
    {
      DataAsset->Version = SDFVersion;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <sdf> has no \"version\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      DataAsset->Version = TEXT("__default__");
    }

  // Iterate <sdf> child nodes
  for (const auto& ChildNode : XmlFile->GetRootNode()->GetChildrenNodes())
    {
      // Check if <model>
      if (ChildNode->GetTag().Equals(TEXT("model")))
        {
          ParseModel(ChildNode);
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <sdf> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <model> node
void FSDFParser::ParseModel(const FXmlNode* InNode)
{
  // Ptr to the new model
  USDFModel* NewModel = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if(!Name.IsEmpty())
    {
      NewModel = NewObject<USDFModel>(DataAsset, FName(*Name));
      NewModel->Name = Name;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <model> has no \"name\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      NewModel = NewObject<USDFModel>(DataAsset/*, FName(TEXT("__default__"))*/);
      NewModel->Name = TEXT("__default__");
    }

  // Iterate <model> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("link")))
        {
          ParseLink(ChildNode, NewModel);
        }
      else if (ChildNode->GetTag().Equals(TEXT("joint")))
        {
          ParseJoint(ChildNode, NewModel);
        }
      else if (ChildNode->GetTag().Equals(TEXT("static")))
        {
          NewModel->bStatic = ChildNode->GetContent().ToBool();
        }
      else if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          NewModel->Pose = PoseContentToFTransform(ChildNode->GetContent());
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <model> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }

  // Add model to the data asset
  DataAsset->Models.Add(NewModel);
}

// Parse <joint> node
void FSDFParser::ParseJoint(const FXmlNode* InNode, USDFModel*& OutModel)
{
  // Pointer to the new joint
  USDFJoint* NewJoint = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
    {
      NewJoint = NewObject<USDFJoint>(OutModel, FName(*Name));
      NewJoint->Name = Name;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> has no \"name\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      NewJoint = NewObject<USDFJoint>(OutModel/*, FName(TEXT("__default__"))*/);
      NewJoint->Name = TEXT("__default__");
    }

  // Get "type" from node attribute
  const FString Type = InNode->GetAttribute(TEXT("type"));
  if (!Name.IsEmpty())
    {
      NewJoint->Type = Type;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> has no \"type\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      NewJoint->Name = TEXT("__default__");
    }

  // Iterate <joint> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("parent")))
        {
          NewJoint->Parent = ChildNode->GetContent();
        }
      else if (ChildNode->GetTag().Equals(TEXT("child")))
        {
          NewJoint->Child = ChildNode->GetContent();
        }
      else if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          NewJoint->Pose = PoseContentToFTransform(ChildNode->GetContent());
          const FString RelativeTo = ChildNode->GetAttribute(TEXT("relative_to"));
          if (!RelativeTo.IsEmpty()) {
            FTransform Relative = FindRelativeTransform(RelativeTo, OutModel);

            FTransform::Multiply(&NewJoint->Pose, &NewJoint->Pose, &Relative);
          }
        }
      else if (ChildNode->GetTag().Equals(TEXT("axis")))
        {
          ParseJointAxis(ChildNode, NewJoint);
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }

  // Add link to the data asset
  OutModel->Joints.Add(NewJoint);
}

// Parse <joint> <axis> node
void FSDFParser::ParseJointAxis(const FXmlNode* InNode, USDFJoint*& OutJoint)
{
  // Iterate <joint> <axis> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("xyz")))
        {
          OutJoint->Axis->Xyz = XyzToFVector(ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("use_parent_model_frame")))
        {
          OutJoint->Axis->bUseParentModelFrame = ChildNode->GetContent().ToBool();
        }
      else if (ChildNode->GetTag().Equals(TEXT("limit")))
        {
          ParseJointAxisLimit(ChildNode, OutJoint);
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> <axis> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <joint> <axis> <limit> node
void FSDFParser::ParseJointAxisLimit(const FXmlNode* InNode, USDFJoint*& OutJoint)
{
  // Iterate <joint> <axis> <limit> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("lower")))
        {
          OutJoint->Axis->SetLowerLimitFromSDF(FCString::Atof(*ChildNode->GetContent()));

        }
      else if (ChildNode->GetTag().Equals(TEXT("upper")))
        {
          OutJoint->Axis->SetUpperLimitFromSDF(FCString::Atof(*ChildNode->GetContent()));
        }
      else if (ChildNode->GetTag().Equals(TEXT("effort")))
        {
          OutJoint->Axis->Effort = FCString::Atof(*ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("velocity")))
        {
          OutJoint->Axis->Velocity = FCString::Atof(*ChildNode->GetContent());
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> <axis> <limit> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <link> <inertial> node
void FSDFParser::ParseLinkInertial(const FXmlNode* InNode, USDFLink*& OutLink)
{
  // Iterate <link> <inertial> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("mass")))
        {
          OutLink->Inertial->Mass = FCString::Atof(*ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          OutLink->Inertial->Pose = PoseContentToFTransform(ChildNode->GetContent());
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <inertial> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <link> node
void FSDFParser::ParseLink(const FXmlNode* InNode, USDFModel*& OutModel)
{
  // Ptr to the new link
  USDFLink* NewLink = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
    {
      NewLink = NewObject<USDFLink>(OutModel, FName(*Name));
      NewLink->Name = Name;
      CurrentLinkName = Name;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> has no \"name\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      NewLink = NewObject<USDFLink>(OutModel/*, FName(TEXT("__default__"))*/);
      NewLink->Name = TEXT("__default__");
    }

  // Iterate <link> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          NewLink->Pose = PoseContentToFTransform(ChildNode->GetContent());
          const FString RelativeTo = ChildNode->GetAttribute(TEXT("relative_to"));
          if (!RelativeTo.IsEmpty()) {
            FTransform Relative = FindRelativeTransform(RelativeTo, OutModel);
            
            FTransform::Multiply(&NewLink->Pose, &NewLink->Pose, &Relative);
          }
        }
      else if (ChildNode->GetTag().Equals(TEXT("inertial")))
        {
          ParseLinkInertial(ChildNode, NewLink);
        }
      else if (ChildNode->GetTag().Equals(TEXT("visual")))
        {
          ParseVisual(ChildNode, NewLink);
        }
      else if (ChildNode->GetTag().Equals(TEXT("collision")))
        {
          ParseCollision(ChildNode, NewLink);
        }
      else if (ChildNode->GetTag().Equals(TEXT("self_collide")))
        {
          NewLink->bSelfCollide = ChildNode->GetContent().ToBool();
        }
      else if (ChildNode->GetTag().Equals(TEXT("gravity")))
        {
          NewLink->bGravity = ChildNode->GetContent().ToBool();
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
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
void FSDFParser::ParseVisual(const FXmlNode* InNode, USDFLink*& OutLink)
{
  // Ptr to the new visual
  USDFVisual* NewVisual = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
    {
      NewVisual = NewObject<USDFVisual>(OutLink, FName(*Name));
      NewVisual->Name = Name;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> has no \"name\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      NewVisual = NewObject<USDFVisual>(OutLink/*, FName(TEXT("__default__"))*/);
      NewVisual->Name = TEXT("__default__");
    }

  // Iterate <visual> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          NewVisual->Pose = PoseContentToFTransform(ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("geometry")))
        {
          ParseGeometry(ChildNode, NewVisual->Geometry, ESDFType::Visual);
          if(NewVisual->Geometry->Type == ESDFGeometryType::Box ||
             NewVisual->Geometry->Type == ESDFGeometryType::Cylinder ||
             NewVisual->Geometry->Type == ESDFGeometryType::Sphere)
            {
              NewVisual->Geometry->Mesh = CreateMesh(ESDFType::Visual, NewVisual->Geometry->Type, Name, RStaticMeshUtils::GetGeometryParameter(NewVisual->Geometry));
            }
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> <visual> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }

  // Add visual to array
  OutLink->Visuals.Add(NewVisual);
}

// Parse <collision> node
void FSDFParser::ParseCollision(const FXmlNode* InNode, USDFLink*& OutLink)
{
  // Ptr to the new collision
  USDFCollision* NewCollision = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
    {
      NewCollision = NewObject<USDFCollision>(OutLink, FName(*Name));
      NewCollision->Name = Name;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <collision> has no \"name\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      NewCollision = NewObject<USDFCollision>(OutLink/*, FName(TEXT("__default__"))*/);
      NewCollision->Name = TEXT("__default__");
    }

  // Iterate <collision> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          NewCollision->Pose = PoseContentToFTransform(ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("geometry")))
        {
          ParseGeometry(ChildNode, NewCollision->Geometry, ESDFType::Collision);
          if(NewCollision->Geometry->Type == ESDFGeometryType::Box ||
             NewCollision->Geometry->Type == ESDFGeometryType::Cylinder ||
             NewCollision->Geometry->Type == ESDFGeometryType::Sphere)
            {
              NewCollision->Geometry->Mesh = CreateMesh(ESDFType::Collision, NewCollision->Geometry->Type, Name, RStaticMeshUtils::GetGeometryParameter(NewCollision->Geometry));
              // RStaticMeshUtils::CreateStaticMeshThroughBrush(OutLink,NewCollision);
            }
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <inertial> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }

  // Add collision to array
  OutLink->Collisions.Add(NewCollision);
}

// Parse <geometry> <box> node
void FSDFParser::ParseGeometryBox(const FXmlNode* InNode, USDFGeometry*& OutGeometry)
{
  // Set geometry type
  OutGeometry->Type = ESDFGeometryType::Box;

  // Iterate <geometry> <box> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("size")))
        {
          OutGeometry->Size = SizeToFVector(ChildNode->GetContent());
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <box> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <geometry> <cylinder> node
void FSDFParser::ParseGeometryCylinder(const FXmlNode* InNode, USDFGeometry*& OutGeometry)
{
  // Set geometry type
  OutGeometry->Type = ESDFGeometryType::Cylinder;

  // Iterate <geometry> <cylinder> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("radius")))
        {
          OutGeometry->Radius = FConversions::MToCm(
                                                    FCString::Atof(*ChildNode->GetContent()));
        }
      else if (ChildNode->GetTag().Equals(TEXT("length")))
        {
          OutGeometry->Length = FConversions::MToCm(
                                                    FCString::Atof(*ChildNode->GetContent()));
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <cylinder> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <geometry> <sphere> node
void FSDFParser::ParseGeometrySphere(const FXmlNode* InNode, USDFGeometry*& OutGeometry)
{
  // Set geometry type
  OutGeometry->Type = ESDFGeometryType::Sphere;

  // Iterate <geometry> <sphere> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("radius")))
        {
          OutGeometry->Radius = FConversions::MToCm(
                                                    FCString::Atof(*ChildNode->GetContent()));
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <sphere> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <geometry> node
void FSDFParser::ParseGeometry(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type)
{
  // Iterate <geometry> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("mesh")))
        {
          ParseGeometryMesh(ChildNode, OutGeometry, Type);
        }
      else if (ChildNode->GetTag().Equals(TEXT("box")))
        {
          ParseGeometryBox(ChildNode, OutGeometry);
        }
      else if (ChildNode->GetTag().Equals(TEXT("cylinder")))
        {
          ParseGeometryCylinder(ChildNode, OutGeometry);
        }
      else if (ChildNode->GetTag().Equals(TEXT("sphere")))
        {
          ParseGeometrySphere(ChildNode, OutGeometry);
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <geometry> <mesh> node
void FSDFParser::ParseGeometryMesh(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type)
{
  // Set geometry type
  OutGeometry->Type = ESDFGeometryType::Mesh;

  // Iterate <geometry> <mesh> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("uri")))
        {
          // Import mesh, set Uri as the relative path from the asset to the mesh uasset
          OutGeometry->Uri = ChildNode->GetContent();
          OutGeometry->Mesh = ImportMesh(OutGeometry->Uri, Type);
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <mesh> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}