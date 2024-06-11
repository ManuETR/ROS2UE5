// Copyright 2017-2021, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "RWCManager.h"


class FROSGetModelSocketPoseServer final : public FROSBridgeSrvServer
{
private:
	FROSGetModelSocketPoseServer();

	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	URWCManager* Controller;

public:
	FROSGetModelSocketPoseServer(FString Namespace, FString Name, UWorld* InWorld,
		URWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/GetModelSocketPose"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
