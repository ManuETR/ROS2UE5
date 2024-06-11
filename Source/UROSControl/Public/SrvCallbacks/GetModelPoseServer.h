// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "RWCManager.h"


class FROSGetModelPoseServer final : public FROSBridgeSrvServer
{
private:
	FROSGetModelPoseServer();

	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	URWCManager* Controller;

public:
	FROSGetModelPoseServer(FString Namespace, FString Name, UWorld* InWorld,
		URWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/GetModelPose"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
