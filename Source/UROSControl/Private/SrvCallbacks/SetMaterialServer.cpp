// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "SrvCallbacks/SetMaterialServer.h"
#include "HAL/FileManagerGeneric.h"
#include "AssetModifier.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSetMaterialServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSSetMaterialSrv::Request> Request =
		MakeShareable(new FROSSetMaterialSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSetMaterialServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSSetMaterialSrv::Request> ChangeVisualRequest =
		StaticCastSharedPtr<FROSSetMaterialSrv::Request>(Request);


	if (ChangeVisualRequest->GetMaterialNames().Num() == 0 &&
		ChangeVisualRequest->GetMaterialPaths().Num() == 0)
	{
		// nothing to do here.
		UE_LOG(LogTemp, Error, TEXT("Nothing to do. %s"), *ChangeVisualRequest->ToString());
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSSetMaterialSrv::Response(false));
	}

	// get Actor with given UtagID of Controller IDMap


	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(World, FName(*ChangeVisualRequest->GetId()), Actors);
	AActor* ActorToBeChanged = Actors.Pop();

	if (ActorToBeChanged)
	{
		TArray<UStaticMeshComponent*> Components;
		ActorToBeChanged->GetComponents<UStaticMeshComponent>(Components);

		bool bOverallSuccess = true;

		for (auto Component : Components)
		{

			// Execute on game thread
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
			{
				ServiceSuccess = FAssetModifier::ChangeVisual(Component, ChangeVisualRequest->GetMaterialNames(), ChangeVisualRequest->GetMaterialPaths());
			}, TStatId(), nullptr, ENamedThreads::GameThread);

			//wait code above to complete
			FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
			bOverallSuccess = bOverallSuccess && ServiceSuccess;
		}

		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSSetMaterialSrv::Response(bOverallSuccess));
	}
	else
	{
		return MakeShareable<FROSBridgeSrv::SrvResponse>
			(new FROSSetMaterialSrv::Response(false));
	}
}
