// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeSrv.h"
#include "RWCManager.h"
#include "world_control_msgs/srv/AttachModelToParent.h"


class FROSAttachModelToParentServer final : public FROSBridgeSrvServer
{
	
private:
	FROSAttachModelToParentServer(){};

	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	URWCManager* Controller;


public:
	FROSAttachModelToParentServer(FString Namespace, FString Name, UWorld* InWorld, URWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/AttachModelToParent"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
