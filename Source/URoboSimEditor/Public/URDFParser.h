// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "SDF/SDFDataAsset.h"
#include "SDF/SDFGeometry.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/ObjectMacros.h" // EObjectFlags
//#include "SDFParserBase.h"
#include "SDFParserInterface.h"

// Forward declaration
class FXmlFile;
class FXmlNode;
class UFbxFactory;
// class USDFDataAsset;
class USDFModel;
class USDFJoint;
class USDFLink;
class USDFVisual;
class USDFCollision;
// class USDFGeometry;



/**
* URDF parser class
*/
class UROBOSIMEDITOR_API FURDFParser :public ISDFParserInterface
{
public:
    // Default constructor
    FURDFParser();

    // Constructor with load from path
    FURDFParser(const FString& InFilename);

    // Destructor
    ~FURDFParser();

    // Load sdf from path
    virtual bool LoadSDF(const FString& InFilename) override;

    // Clear parser
    virtual void Clear() override;

    // Create data asset and parse sdf data into it
    virtual USDFDataAsset* ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags)override;

private:
    /* Begin parser functions */
    // Check if sdf data is valid
    virtual bool IsValidSDF() override;

    // Parse <sdf> node
    virtual void ParseSDF() override;

    //Get ROS package path
    virtual FString GetROSPackagePath(const FString& InPackageName);
    virtual void GetROSPackagePaths();

    // Parse <model> node
    // virtual void ParseModel(const FXmlNode* InNode) override;
    // Parse <link> node
    virtual void ParseLink(const FXmlNode* InNode, USDFModel*& OutModel) override;
    // virtual void ParsePlugin(const FXmlNode* InNode, USDFModel*& OutModel) ;
    // Parse <link> <inertial> node
    virtual void ParseLinkInertial(const FXmlNode* InNode, USDFLink*& OutLink) override;
    // Parse <visual> node
    virtual void ParseVisual(const FXmlNode* InNode, USDFLink*& OutLink) override;
    // Parse <collision> node
    virtual void ParseCollision(const FXmlNode* InNode, USDFLink*& OutLink) override;
    // Parse <geometry> node
    virtual void ParseGeometry(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type) override;
    // Parse <geometry> <mesh> node
    virtual void ParseGeometryMesh(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type) override;
    // Parse <geometry> <box> node
    virtual void ParseGeometryBox(const FXmlNode* InNode, USDFGeometry*& OutGeometry) override;
    // Parse <geometry> <cylinder> node
    virtual void ParseGeometryCylinder(const FXmlNode* InNode, USDFGeometry*& OutGeometry) override;
    // Parse <geometry> <sphere> node
    virtual void ParseGeometrySphere(const FXmlNode* InNode, USDFGeometry*& OutGeometry) override;
    // Parse <joint> node
    virtual void ParseJoint(const FXmlNode* InNode, USDFModel*& OutModel) override;
    // Parse <joint> <axis> node
    virtual void ParseJointAxis(const FXmlNode* InNode, USDFJoint*& OutJoint) override;
    // Parse <joint> <axis> <limit> node
    virtual void ParseJointAxisLimit(const FXmlNode* InNode, USDFJoint*& OutJoint) override;
    /* End parser functions */


    void SetDirectoryPath(const FString& InFilename);

    FString GetMeshAbsolutePath(const FString& Uri);

    FName GenerateMeshName(ESDFType InType, FString InName);
    FString GeneratePackageName(FName MeshName);
    bool CreateCollisionForMesh(UStaticMesh* OutMesh, ESDFGeometryType Type);
    USDFCollision* CreateVirtualCollision(USDFLink* OutLink);

    UStaticMesh* ImportMesh(const FString& Uri, ESDFType Type);
    UStaticMesh* CreateMesh(ESDFType InType, ESDFGeometryType InShape, FString InName, TArray<float> InParameters);

    TMap<FString,FString> ROSPackagePaths;

    USDFDataAsset* DataAsset;

    FString DirPath;

    FString CurrentLinkName;

    UFbxFactory* FbxFactory;

    FAssetRegistryModule& AssetRegistryModule;
};
