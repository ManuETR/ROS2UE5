// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "SrvCallbacks/HighlightModelsServer.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "AssetHighlighter.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSHighlightModelsServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSHighlightModelSrv::Request> Request_ =
		MakeShareable(new FROSHighlightModelSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSHighlightModelsServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSHighlightModelSrv::Request> HighlightModelRequest =
		StaticCastSharedPtr<FROSHighlightModelSrv::Request>(Request);

	bool bSuccess = true;
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsWithTag(World, FName(*HighlightModelRequest->GetId()), Actors);

		if (Actors.IsValidIndex(0))
		{
			AActor* ActorToBeHighlighted = Actors.Pop();
			// Execute on game thread

			FAssetHighlighter::Highlight(ActorToBeHighlighted, HighlightModelRequest->GetColor());
		}
		else
		{
			bSuccess = false;
		}
	}, TStatId(), nullptr, ENamedThreads::GameThread);


	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSHighlightModelSrv::Response(bSuccess));
}
