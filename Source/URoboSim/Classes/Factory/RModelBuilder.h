// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "Physics/RModel.h"
#include "RobotDescription/RDDataAsset.h"
#include "Factory/RLinkFactory.h"
#include "Factory/RJointFactory.h"
#include "Physics/RLink.h"
#include "RModelBuilder.generated.h"


class URDModel;
class URDJoint;
class URDLink;
class URJoint;
// class URLink;

UCLASS()
class UROBOSIM_API URModelBuilder : public UObject
{
  GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    URModelBuilder();

  // Destructor
  ~URModelBuilder();

  UPROPERTY()
    URJointFactory* JointFactory;

  UPROPERTY()
    URLinkFactory* LinkFactory;

  // Load model
  void Load(URDModel* InModelDescription, ARModel* OutModel,FVector InLocation);
  void AddController(ARModel* OutModel);
protected:
  // Called when the game starts or when spawned

  UPROPERTY()
  URDModel* ModelDescription;

  UPROPERTY()
  ARModel* Model;

  void LoadLinks(FVector InLocation);
  void LoadJoints();
  void BuildKinematicTree();
  void SetupPlugins();
  void SetupROS();
  void SetupControl();
  void SetupLogger();
  void SetConstraintPosition(URJoint* InJoint);

  void AddJoint(URJoint* Joint);
  void AddLink(URLink* Link);
};
