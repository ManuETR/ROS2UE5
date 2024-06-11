// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "RWCManager.h"
#include "world_control_msgs/srv/HighlightModel.h"


class FROSHighlightModelsServer final : public FROSBridgeSrvServer
{

private:
	FROSHighlightModelsServer();

	UWorld* World;

	URWCManager* Controller;
public:
	FROSHighlightModelsServer(FString Namespace, FString Name, UWorld* InWorld,
		URWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/HighlightModel")),
		World(InWorld), Controller(InController)
	{
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
