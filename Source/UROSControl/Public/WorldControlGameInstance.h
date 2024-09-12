// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UROSBridge/Public/ROSBridgeGameInstance.h"
#include "RWCManager.h"
#include "WorldControlGameInstance.generated.h"

/**
 *
 */
UCLASS()
class UROSCONTROL_API UWorldControlGameInstance : public UROSBridgeGameInstance
{
  GENERATED_BODY()

 public:
    virtual void OnStart() override;

  UWorldControlGameInstance();

  UPROPERTY(EditAnywhere, Instanced)
    URWCManager* Manager;

  UPROPERTY(EditAnywhere, Category = "ROS")
    FString Namespace = FString("UnrealSim");
};
