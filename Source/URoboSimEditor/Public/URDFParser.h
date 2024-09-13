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

    // Create data asset and parse urdf data into it
    virtual URDDataAsset* ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags) override;

    virtual void Parse() override;


private:
  // Parse <link> node
  virtual void ParseLink(const FXmlNode* InNode, URDModel*& OutModel);

  // Parse <link> <inertial> node
  virtual void ParseLinkInertial(const FXmlNode* InNode, URDLink*& OutLink);

  // Parse <visual> node
  virtual void ParseVisual(const FXmlNode* InNode, URDLink*& OutLink);

  // Parse <collision> node
  virtual void ParseCollision(const FXmlNode* InNode, URDLink*& OutLink);

  // Parse <geometry> node
  virtual void ParseGeometry(const FXmlNode* InNode, URDGeometry*& OutGeometry, ERDType Type);

  // Parse <geometry> <mesh> node
  virtual void ParseGeometryMesh(const FXmlNode* InNode, URDGeometry*& OutGeometry, ERDType Type);

  // Parse <geometry> <box> node
  virtual void ParseGeometryBox(const FXmlNode* InNode, URDGeometry*& OutGeometry);

  // Parse <geometry> <cylinder> node
  virtual void ParseGeometryCylinder(const FXmlNode* InNode, URDGeometry*& OutGeometry);

  // Parse <geometry> <sphere> node
  virtual void ParseGeometrySphere(const FXmlNode* InNode, URDGeometry*& OutGeometry);

  // Parse <joint> node
  virtual void ParseJoint(const FXmlNode* InNode, URDModel*& OutModel);

  // Parse <joint> <axis> node
  virtual void ParseJointAxis(const FXmlNode* InNode, URDJoint*& OutJoint);

  // Parse <joint> <axis> <limit> node
  virtual void ParseJointAxisLimit(const FXmlNode* InNode, URDJoint*& OutJoint);

  bool IsValid();
};
