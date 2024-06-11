// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen
#include "SrvCallbacks/GetModelPoseServer.h"
#include "world_control_msgs/srv/GetModelPose.h"
#include "Conversions.h"


TSharedPtr<FROSBridgeSrv::SrvRequest> FROSGetModelPoseServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSGetModelPoseSrv::Request> Request_ =
		MakeShareable(new FROSGetModelPoseSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSGetModelPoseServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSGetModelPoseSrv::Request> GetModelPoseRequest =
		StaticCastSharedPtr<FROSGetModelPoseSrv::Request>(Request);

	// Do all calculations that don't need a game thread first

	//Get Actor for given ID
	FString UniqueId = GetModelPoseRequest->GetId();

  geometry_msgs::Pose Pose;

	double start = FPlatformTime::Seconds();
// 	//The rest has to be done in the game thread
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		// GetActorsWithKeyValuePair (which internally uses TActorIterator) needs a game thread
			
		TArray<AActor*> AllMatchingActors;
		UGameplayStatics::GetAllActorsWithTag(World, FName(*UniqueId), AllMatchingActors);
		AActor* Actor = nullptr;
		if(AllMatchingActors.Num() > 0)
			Actor = AllMatchingActors.Pop();

		if (!Actor)
		{
			// Couldn't find Actor for ID
			UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist."), *UniqueId);
			ServiceSuccess = false;
			return;
		}
		Pose.SetPosition(geometry_msgs::Point( FConversions::UToROS(Actor->GetActorLocation() )));
		Pose.SetOrientation(geometry_msgs::Quaternion( FConversions::UToROS(Actor->GetActorRotation().Quaternion()) ));

		ServiceSuccess = true;
	}, TStatId(), nullptr, ENamedThreads::GameThread);
	// wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	double end = FPlatformTime::Seconds();
	UE_LOG(LogTemp, Display, TEXT("GetModelPose executed in %f seconds."), end-start);


	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSGetModelPoseSrv::Response(ServiceSuccess, Pose));
}
