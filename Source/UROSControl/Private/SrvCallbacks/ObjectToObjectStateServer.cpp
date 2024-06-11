#include "SrvCallbacks/ObjectToObjectStateServer.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSObjectToObjectStateServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSObjectToObjectSrv::Request> Request_ =
		MakeShareable(new FROSObjectToObjectSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSObjectToObjectStateServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSObjectToObjectSrv::Request> ObjectStateRequest =
		StaticCastSharedPtr<FROSObjectToObjectSrv::Request>(Request);

	FString SourceObjectId = ObjectStateRequest->GetSourceObjectId();
	FString OtherObjectId = ObjectStateRequest->GetOtherObjectId();

	FString Output = FString();

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			// InWorld Actions and Calculations
			AActor* SourceObject;
			AActor* OtherObject;

			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsWithTag(World, FName(*SourceObjectId), Actors);

			if (Actors.Num() != 0)
			{
				SourceObject = Actors[0];
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s: Source Object with Id \"%s\" not found"), __FUNCTION__, *SourceObjectId);
				ServiceSuccess = false;
				return;
			}

			UGameplayStatics::GetAllActorsWithTag(World, FName(*OtherObjectId), Actors);
			if (Actors.Num() != 0)
			{
				OtherObject = Actors[0];
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s: Other Object with Id \"%s\" not found"), __FUNCTION__, *OtherObjectId);
				ServiceSuccess = false;
				return;
			}

			// FVector VectorToObject = GetVectorFromAToB(OriginObject, ObjectToCheckFor); // Doesn't work
			FVector VectorToObject = SourceObject->GetActorLocation() - OtherObject->GetActorLocation();
			float ActorBoxSizeHeightZ = SourceObject->GetComponentsBoundingBox().GetExtent().Z;
			float ActorBoxSizeHeightY = SourceObject->GetComponentsBoundingBox().GetExtent().Y;
			float ActorBoxSizeHeightX = SourceObject->GetComponentsBoundingBox().GetExtent().X;

			// Many cases are possible at once, but priotiy is from weakest to strongest
			// Inside, On = Under, Above = Below
			// WIP: Alternative Solution for IN
			bool bInside = VectorToObject.Z <= ActorBoxSizeHeightZ && VectorToObject.Z >= -ActorBoxSizeHeightZ
				&& VectorToObject.Y <= ActorBoxSizeHeightY && VectorToObject.Y >= -ActorBoxSizeHeightY
				&& VectorToObject.X <= ActorBoxSizeHeightX && VectorToObject.X >= -ActorBoxSizeHeightX;
			bool bOn = false; // WIP: TODO Later
			bool bUnder = false; // WIP: TODO Later
			bool bAbove = VectorToObject.Z >= 0;
			bool bBelow = VectorToObject.Z < 0;

			// Start Building Output
			if (bInside && Output.IsEmpty())
				Output = FString("INSIDE");
			if (bOn && Output.IsEmpty())
				Output = FString("ON");
			if (bUnder && Output.IsEmpty())
				Output = FString("UNDER");
			if (bAbove && Output.IsEmpty())
				Output = FString("ABOVE");
			if (bBelow && Output.IsEmpty())
				Output = FString("BELOW");

			ServiceSuccess = true;
		}, TStatId(), nullptr, ENamedThreads::GameThread);

	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	return TSharedPtr<FROSBridgeSrv::SrvResponse>
		(new FROSObjectToObjectSrv::Response(Output, ServiceSuccess));
}
