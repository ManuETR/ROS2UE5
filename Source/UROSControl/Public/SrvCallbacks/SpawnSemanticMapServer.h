#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "world_control_msgs/srv/SpawnSemanticMap.h"
#include "RWCManager.h"


class FROSSpawnSemanticMapServer : public FROSBridgeSrvServer
{
protected:

	FROSSpawnSemanticMapServer()
	{
	};

	UWorld* World;

	FThreadSafeBool bAllSuccessfull;
private:

public:
	URWCManager* Controller;

	FROSSpawnSemanticMapServer(FString Namespace, FString Name, UWorld* InWorld, URWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/SpawnSemanticMap"))
	{
		Controller = InController;
		World = InWorld;
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
