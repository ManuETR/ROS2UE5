// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "RWCManager.h"
#include "world_control_msgs/srv/SpawnPhysicsConstraint.h"
#include "PhysicsEngine/ConstraintInstance.h"


class FROSSpawnPhysicsConstraintServer final : public FROSBridgeSrvServer
{
	
private:
	FROSSpawnPhysicsConstraintServer();

	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	URWCManager* Controller;

	bool SpawnPhysicsConstraintActor(TSharedPtr<FROSSpawnPhysicsConstraintSrv::Request> Request);


public:
	FROSSpawnPhysicsConstraintServer(FString Namespace, FString Name, UWorld* InWorld,
				URWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/SpawnPhysicsConstraint"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
