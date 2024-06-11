// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen
#include "SrvCallbacks/GetModelSocketPoseServer.h"
#include "world_control_msgs/srv/GetModelSocketPose.h"
#include "Conversions.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSGetModelSocketPoseServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSGetModelSocketPoseSrv::Request> Request_ =
		MakeShareable(new FROSGetModelSocketPoseSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSGetModelSocketPoseServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSGetModelSocketPoseSrv::Request> GetModelSocketPoseRequest =
		StaticCastSharedPtr<FROSGetModelSocketPoseSrv::Request>(Request);

	// Do all calculations that don't need a game thread first

	//Get Actor for given ID
	FString UniqueId = GetModelSocketPoseRequest->GetId();
	
	FString Tag = GetModelSocketPoseRequest->GetSocket();
        FName ConvertedFStringTag = FName(*Tag);	
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
		
		TArray<UStaticMeshComponent*> Components;
 		Actor->GetComponents<UStaticMeshComponent>(Components);
		for( int32 i=0; i<Components.Num(); i++ )
 		{
		  //iterate over component in static mesh
   		  UStaticMeshComponent* StaticMeshComponent = Components[i];
  		  UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();	
		  //get specific socket	
		  UStaticMeshSocket* Socket = StaticMesh->FindSocket(ConvertedFStringTag);
		  if (!Socket)
		  {
			// Couldn't find Actor for ID
			UE_LOG(LogTemp, Warning, TEXT("Socket with id:\"%s\" does not exist."), *Tag);
			ServiceSuccess = false;
			return;
		  }
		  	
		  FTransform OutTransform;	
		  Socket->GetSocketTransform(OutTransform, StaticMeshComponent);

		  const FVector& lidTobj = OutTransform.GetLocation();

		  //add location from the mesh to socket location, results into socket location in UE_World  frame
		  FVector annoTwor = Actor->GetActorLocation();
		  FVector lidTwor = FVector(annoTwor.X, annoTwor.Y, annoTwor.Z);
		  lidTwor.X += lidTobj.X;
		  lidTwor.Y += lidTobj.Y;
		  lidTwor.Z += lidTobj.Z;
		  const FVector& test = lidTwor;

		  //Conversion from Vector to ros msgs pose	
		  FVector tmpVecSocket = FConversions::UToROS(lidTobj);
		  geometry_msgs::Point pp = tmpVecSocket;
		  Pose.SetPosition(pp);
		  
				
	          UE_LOG(LogTemp, Display, TEXT("socket realativ %s"), *lidTobj.ToString());	
		  UE_LOG(LogTemp, Display, TEXT("actor realativ %s"), *lidTwor.ToString());		
 		}
		//set orientation from the actor (socket is always described with the actor so we can do this)
		Pose.SetOrientation(geometry_msgs::Quaternion( FConversions::UToROS(Actor->GetActorRotation().Quaternion()) ));

		ServiceSuccess = true;
	}, TStatId(), nullptr, ENamedThreads::GameThread);
	// wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	double end = FPlatformTime::Seconds();
	UE_LOG(LogTemp, Display, TEXT("GetModelSocketPose executed in %f seconds."), end-start);


	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSGetModelSocketPoseSrv::Response(ServiceSuccess, Pose));
}
