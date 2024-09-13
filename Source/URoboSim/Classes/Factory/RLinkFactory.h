// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann (neumann.michael@uni-bremen.de)

#pragma once

#include "CoreMinimal.h"
#include "Physics/RLink.h"
#include "Physics/RModel.h"
#include "RobotDescription/RDLink.h"
#include "RLinkFactory.generated.h"


UCLASS()
class UROBOSIM_API URLinkBuilder : public UObject
{
  GENERATED_BODY()

public:

  virtual void  Init(UObject* InOuter, URDLink* InLinkDescription);
  virtual void Init(UObject* InOuter, URDLink* InLinkDescription,FVector InLocation);
//  virtual URLink* NewLink(FVector InLocation);
  virtual URLink* NewLink();

  UPROPERTY()
    ARModel* Model;

  UPROPERTY()
  FVector LoadLocation;
protected:


  UPROPERTY()
    URDLink* LinkDescription;

  UPROPERTY()
    URLink* Link;

  FTransform LinkPose;

  void SetPose(FTransform InPose);
  void SetPoseComponent();

  virtual void SetVisuals();
  virtual void SetVisual(URDVisual* InVisual);
  virtual void SetCollisions();
  virtual void SetCollision(URDCollision* InCollision);

  virtual void SetInertial(URDLinkInertial* InInertial);
  virtual void SetCollisionProfile(bool InSelfColide);
  virtual void SetSimulateGravity(bool InUseGravity);
};

UCLASS()
class UROBOSIM_API URLinkFactory : public UObject
{
  GENERATED_BODY()
 public:

    URLink* Load(UObject* InOuter, URDLink* InLinkDescription);
    URLink* Load(UObject* InOuter, URDLink* InLinkDescription,FVector InLoaction);

protected:

  virtual URLinkBuilder* CreateBuilder(URDLink* InLinkDescription);

  UPROPERTY()
    URLinkBuilder* LinkBuilder;

};
