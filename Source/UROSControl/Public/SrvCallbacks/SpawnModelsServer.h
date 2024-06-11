// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
//#include "worldSpawnModel.h"
#include "world_control_msgs/srv/SpawnModel.h"
#include "world_control_msgs/msgs/PhysicsProperties.h"
#include "RWCManager.h"
#include "Engine/StaticMesh.h"


class FROSSpawnModelServer : public FROSBridgeSrvServer
{
protected:

	FROSSpawnModelServer()
	{
	};

	UWorld* World;

	FThreadSafeBool ServiceSuccess;


private:
	UStaticMesh* LoadMesh(FString Name, FString StartDir);
	UMaterialInterface* LoadMaterial(FString Name, FString StartDir);


public:
	URWCManager* Controller;

	FROSSpawnModelServer(FString Namespace, FString Name, UWorld* InWorld, URWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/SpawnModel"))
	{
		Controller = InController;
		World = InWorld;
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
