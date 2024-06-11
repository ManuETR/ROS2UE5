#pragma once
#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "RWCManager.h"
#include "world_control_msgs/srv/ObjectToObjectState.h"

class FROSObjectToObjectStateServer final : public FROSBridgeSrvServer
{
private:
	FROSObjectToObjectStateServer();
	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	URWCManager* Controller;

public:
	FROSObjectToObjectStateServer(FString Namespace, FString Name, UWorld* InWorld,
		URWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/ObjectToObjectState"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
