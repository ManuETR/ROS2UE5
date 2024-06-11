// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "RWCManager.h"
#include "world_control_msgs/srv/SpawnProMesh.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
//#include "KismetProceduralMeshLibrary.h"
#include "RWCManager.h"
//#include "ProceduralMeshComponent.h"
//#include "CustomMeshComponent.h"
#include "BoundingBox.h"
/**
 *
 */

struct FPhysicsProperties
{
    EComponentMobility::Type Mobility;
    bool bSimulatePhysics;
    bool bGravity;
    bool bGenerateOverlapEvents;
    float Mass;
};
class UROSCONTROL_API FROSSpawnProMeshServer : public FROSBridgeSrvServer
{
public:
    FROSSpawnProMeshServer();

    UWorld* World;

    FThreadSafeBool ServiceSuccess;

private:
    ABoundingBox* SpawnProMesh(TSharedPtr<FROSSpawnProMeshSrv::Request> SpawnProMeshRequest);

public:
    URWCManager* Controller;

    FROSSpawnProMeshServer(FString Namespace, FString Name, UWorld* InWorld, URWCManager* InController) :
        FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/SpawnProMesh"))
    {
        Controller = InController;
        World = InWorld;
    }


    TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

    TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};

