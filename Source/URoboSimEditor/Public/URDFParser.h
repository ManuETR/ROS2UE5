#pragma once

#include "AbstractRobotDescriptionParser.h"



/**
* URDF parser class
*/
class UROBOSIMEDITOR_API FURDFParser : public FAbstractRobotDescriptionParser
{
public:
    // Default constructor
    FURDFParser();

    // Constructor with load from path
    FURDFParser(const FString& InFilename);

    // Destructor
    ~FURDFParser();

    virtual bool Load(const FString& InFilename) override;

    // Clear parser
    virtual void Clear() override;

    // Create data asset and parse sdf data into it
    virtual USDFDataAsset* ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags) override;

    virtual void Parse() override;


private:
  // Parse <link> node
  virtual void ParseLink(const FXmlNode* InNode, USDFModel*& OutModel);

  // Parse <link> <inertial> node
  virtual void ParseLinkInertial(const FXmlNode* InNode, USDFLink*& OutLink);

  // Parse <visual> node
  virtual void ParseVisual(const FXmlNode* InNode, USDFLink*& OutLink);

  // Parse <collision> node
  virtual void ParseCollision(const FXmlNode* InNode, USDFLink*& OutLink);

  // Parse <geometry> node
  virtual void ParseGeometry(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type);

  // Parse <geometry> <mesh> node
  virtual void ParseGeometryMesh(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type);

  // Parse <geometry> <box> node
  virtual void ParseGeometryBox(const FXmlNode* InNode, USDFGeometry*& OutGeometry);

  // Parse <geometry> <cylinder> node
  virtual void ParseGeometryCylinder(const FXmlNode* InNode, USDFGeometry*& OutGeometry);

  // Parse <geometry> <sphere> node
  virtual void ParseGeometrySphere(const FXmlNode* InNode, USDFGeometry*& OutGeometry);

  // Parse <joint> node
  virtual void ParseJoint(const FXmlNode* InNode, USDFModel*& OutModel);

  // Parse <joint> <axis> node
  virtual void ParseJointAxis(const FXmlNode* InNode, USDFJoint*& OutJoint);

  // Parse <joint> <axis> <limit> node
  virtual void ParseJointAxisLimit(const FXmlNode* InNode, USDFJoint*& OutJoint);

  bool IsValid();
};
