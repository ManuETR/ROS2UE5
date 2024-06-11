// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "SrvCallbacks/RemoveModelServer.h"
#include "AssetModifier.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSRemoveModelServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSDeleteModelSrv::Request> Request_ =
		MakeShareable(new FROSDeleteModelSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSRemoveModelServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSDeleteModelSrv::Request> RemoveModelRequest =
		StaticCastSharedPtr<FROSDeleteModelSrv::Request>(Request);


		// Actor was found and will be destroyed on GameThread
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			ServiceSuccess = FAssetModifier::RemoveAsset(World, RemoveModelRequest->GetId());
		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

		return TSharedPtr<FROSBridgeSrv::SrvResponse>
			(new FROSDeleteModelSrv::Response(ServiceSuccess));
}
