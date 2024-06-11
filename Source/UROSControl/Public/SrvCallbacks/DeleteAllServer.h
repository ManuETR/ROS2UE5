#pragma once
#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "RWCManager.h"
#include "world_control_msgs/srv/DeleteAll.h"

class FROSDeleteAllServer final : public FROSBridgeSrvServer
{
private:
	FROSDeleteAllServer();
	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	URWCManager* Controller;

public:
	FROSDeleteAllServer(FString Namespace, FString Name, UWorld* InWorld,
			URWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/DeleteAll"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
