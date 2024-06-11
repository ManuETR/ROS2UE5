// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "RWCManager.h"


class FROSSetModelPoseServer final : public FROSBridgeSrvServer
{
	struct MoveAssetParams
	{
		AActor* Actor;
		FVector Location;
		FRotator Rotator;
	};

private:
	FROSSetModelPoseServer();

	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	URWCManager* Controller;

	bool Relocate(AActor* Actor, FVector Location, FRotator Rotator);

public:
	FROSSetModelPoseServer(FString Namespace, FString Name, UWorld* InWorld,
		URWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/SetModelPose"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
