// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StaticMesh.h"
#include "RobotDescription/RDDataAsset.h"
#include "RobotDescription/RDGeometry.h"
#include "RobotDescription/RDModel.h"
#include "RobotDescription/RDLink.h"
#include "RobotDescription/RDJoint.h"
#include "RobotDescription/RDCollision.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "RStaticMeshEditUtils.h"
#include "UObject/ObjectMacros.h" // EObjectFlags
#include "Factories/FbxFactory.h"
#include "XmlNode.h"
#include "XmlFile.h"

/**
*
*/
enum class ERDType : uint8
{
  None,
  Visual,
  Collision
};


/**
 * abstract robot description parser
 */
class UROBOSIMEDITOR_API FAbstractRobotDescriptionParser
{
public:
  FAbstractRobotDescriptionParser();
  virtual bool Load(const FString& InFilename) = 0;

  // Clear parser
  virtual void Clear() = 0;

  // Create data asset and parse data into it
  virtual URDDataAsset* ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags) = 0;

  virtual void Parse() = 0;

  // From <pose>z y z r p y</pose> to FTransform
  virtual FTransform PoseContentToFTransform(const FString& InPoseData);

  // Find the FTransform from the relative to Element
  virtual FTransform FindRelativeTransform(const FString RelativeTo, URDModel* Model);

  virtual URDJoint* FindJoint(const FString Needle, URDModel* Model);
  virtual URDLink* FindLink(const FString Needle, URDModel* Model);

  // From <size>z y z</size> to FVector
  virtual FVector SizeToFVector(const FString& InSizeData);

  // From <xzy>z y z</xzy> to FVector
  virtual FVector XyzToFVector(const FString& InXyzData);


  FAssetRegistryModule& AssetRegistryModule;

protected: //Variables
  // Reader for the xml file
  FXmlFile* XmlFile;

  // Flag if parser is loaded
  bool bLoaded;

  virtual FString GetROSPackagePath(const FString& InPackageName);
  virtual void GetROSPackagePaths();

  // Fix file path
  void SetDirectoryPath(const FString& InFilename);

  // Get mesh absolute path
  FString GetMeshAbsolutePath(const FString& Uri);

  FName GenerateMeshName(ERDType InType, FString InName);
  FString GeneratePackageName(FName MeshName);
  bool CreateCollisionForMesh(UStaticMesh* OutMesh, ERDGeometryType Type);
  URDCollision* CreateVirtualCollision(URDLink* OutLink);

  // Import .fbx meshes from data asset
  UStaticMesh* ImportMesh(const FString& Uri, ERDType Type);
  UStaticMesh* CreateMesh(ERDType InType, ERDGeometryType InShape, FString InName, TArray<float> InParameters);


  TMap<FString, FString> ROSPackagePaths;

  // Pointer to the generated data asset
  URDDataAsset* DataAsset;

  // Cached directory path
  FString DirPath;

  FString CurrentLinkName;

  // Fbx factory
  UFbxFactory* FbxFactory;

};
