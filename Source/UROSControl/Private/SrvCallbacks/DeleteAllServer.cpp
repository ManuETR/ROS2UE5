#include "SrvCallbacks/DeleteAllServer.h"
#include "AssetModifier.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSDeleteAllServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSDeleteAllSrv::Request> Request_ =
		MakeShareable(new FROSDeleteAllSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSDeleteAllServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSDeleteAllSrv::Request> RemoveAllRequest =
		StaticCastSharedPtr<FROSDeleteAllSrv::Request>(Request);

	// Do all calculations that don't need a game thread first.

	// Get the important values from the Request.
	FString ReqType = RemoveAllRequest->GetType();
	FString ReqKey = RemoveAllRequest->GetKey();
	FString ReqValue = RemoveAllRequest->GetValue();
	bool ignoreValue = RemoveAllRequest->GetIgnore();

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			ServiceSuccess = true;

			// Start of finding all matching actors, taking bool in consideration if value included or not.

			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsWithTag(World, FName(*ReqType), Actors);
			for (AActor* Actor : Actors)
			{
				ServiceSuccess = ServiceSuccess && Actor->Destroy();
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);

	// Wait code above to finish
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	return TSharedPtr<FROSBridgeSrv::SrvResponse>
		(new FROSDeleteAllSrv::Response(ServiceSuccess));
};
