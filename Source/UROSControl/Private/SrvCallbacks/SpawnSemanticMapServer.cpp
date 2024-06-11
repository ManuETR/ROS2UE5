// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "SrvCallbacks/SpawnSemanticMapServer.h"
#include "Ids.h"
#include "Conversions.h"
#include "AssetSpawner.h"
#include "ConstraintSpawner.h"
#include "AssetModifier.h"


TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSpawnSemanticMapServer::FromJson(
	TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSSpawnSemanticMapSrv::Request> Request =
		MakeShareable(new FROSSpawnSemanticMapSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSpawnSemanticMapServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSSpawnSemanticMapSrv::Request> SpawnSemanticMapRequest =
		StaticCastSharedPtr<FROSSpawnSemanticMapSrv::Request>(Request);

	TArray<FString> FailedModelIds;
	TArray<FString> FailedConstraintIds;
	TArray<world_control_msgs::RelationDescription> FailedRelations;
	FThreadSafeBool bSuccess = true;


	// Spawn all models
	for (auto ModelDescription : SpawnSemanticMapRequest->GetModels())
	{
		FAssetSpawner::FSpawnAssetParams Params;
		Params.Id = ModelDescription.GetId().IsEmpty() ? FIds::NewGuidInBase64() : ModelDescription.GetId();
		Params.Name = ModelDescription.GetName();
		Params.Location = FConversions::ROSToU(ModelDescription.GetPose().GetPosition().GetVector());
		Params.Rotator = FRotator(FConversions::ROSToU(ModelDescription.GetPose().GetOrientation().GetQuat()));


		Params.PhysicsProperties.Mobility = ModelDescription.GetPhysicsProperties().GetMobility();
		Params.PhysicsProperties.bSimulatePhysics = ModelDescription.GetPhysicsProperties().IsSimulatePhysics();

		Params.PhysicsProperties.bGravity = ModelDescription.GetPhysicsProperties().GetGravity();
		Params.PhysicsProperties.bGenerateOverlapEvents = ModelDescription.GetPhysicsProperties().GetGenerateOverlapEvents();
		Params.PhysicsProperties.Mass = ModelDescription.GetPhysicsProperties().GetMass();
		Params.StartDir = ModelDescription.GetPath();
		Params.ActorLabel = ModelDescription.GetActorLabel();
		Params.MaterialNames = ModelDescription.GetMaterialNames();
		Params.MaterialPaths = ModelDescription.GetMaterialPaths();;


		// Execute on game thread
    FString FinalActorName;
	FString ErrType;
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			bSuccess = FAssetSpawner::SpawnAsset(World, Params, FinalActorName, ErrType);
		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

		bAllSuccessfull.AtomicSet(bAllSuccessfull ? bSuccess : bAllSuccessfull);
		if (!bSuccess)
		{
			FailedModelIds.Add(Params.Id);
		}

	}

	// Spawn all Constraints
	for (auto ConstraintDescription : SpawnSemanticMapRequest->GetConstraints())
	{
		// Setup liniear limits
		FConstraintSpawner::FLinearLimits LinLimits;
		world_control_msgs::LinearLimits LinLimitsMsg = ConstraintDescription.GetConstraintDetails().GetLinearLimits();

		LinLimits.XMotion = LinLimitsMsg.GetXMotion();
		LinLimits.YMotion = LinLimitsMsg.GetYMotion();
		LinLimits.ZMotion = LinLimitsMsg.GetZMotion();
		LinLimits.Limit = FConversions::MToCm(LinLimitsMsg.GetLimit());
		LinLimits.UseAdvanced = LinLimitsMsg.GetUseAdvanced();
		LinLimits.SoftConstraint = LinLimitsMsg.GetSoftConstraint();
		LinLimits.Stiffness = LinLimitsMsg.GetStiffness();
		LinLimits.Damping = LinLimitsMsg.GetDamping();


		// Setup angular limits
		FConstraintSpawner::FAngularLimits AngLimits;
		world_control_msgs::AngularLimits AngLimitsMsg = ConstraintDescription.GetConstraintDetails().GetAngularLimits();

		AngLimits.Swing1Motion = AngLimitsMsg.GetSwing1Motion();
		AngLimits.Swing2Motion = AngLimitsMsg.GetSwing2Motion();
		AngLimits.TwistMotion = AngLimitsMsg.GetTwistMotion();
		AngLimits.Swing1LimitAngle = FMath::RadiansToDegrees(AngLimitsMsg.GetSwing1LimitAngle());
		AngLimits.Swing2LimitAngle = FMath::RadiansToDegrees(AngLimitsMsg.GetSwing2LimitAngle());
		AngLimits.TwistLimitAngle = FMath::RadiansToDegrees(AngLimitsMsg.GetTwistLimitAngle());
		AngLimits.AngularRoationOffset = AngLimitsMsg.GetAngularRoationOffset().GetVector();
		AngLimits.UseAdvanced = AngLimitsMsg.GetUseAdvanced();
		AngLimits.SwingSoftConstraint = AngLimitsMsg.GetSwingSoftConstraint();
		AngLimits.SwingStiffness = AngLimitsMsg.GetSwingStiffness();
		AngLimits.SwingDamping = AngLimitsMsg.GetSwingDamping();
		AngLimits.TwistSoftConstraint = AngLimitsMsg.GetTwistSoftConstraint();
		AngLimits.TwistStiffness = AngLimitsMsg.GetTwistStiffness();
		AngLimits.TwistDamping = AngLimitsMsg.GetTwistDamping();


		// Setup Constraint details
		FConstraintSpawner::FPhysicsConstraintDetails Details;
		world_control_msgs::PhysicsConstraintDetails DetailsMsg = ConstraintDescription.GetConstraintDetails();

		Details.IdFirstModel = DetailsMsg.GetIdFirstModel();
		Details.IdSecondModel = DetailsMsg.GetIdSecondModel();
		Details.DisableCollision = DetailsMsg.GetDisableCollision();
		Details.EnableProjection = DetailsMsg.GetEnableProjection();
		Details.ProjectionLinearTolerance = DetailsMsg.GetProjectionLinearTolerance();
		Details.ProjectionAngularTolerance = DetailsMsg.GetProjectionAngularTolerance();
		Details.ParentDominates = DetailsMsg.GetParentDominates();
		Details.LinearLimits = LinLimits;
		Details.AngularLimits = AngLimits;


		FVector Location = FConversions::ROSToU(ConstraintDescription.GetPose().GetPosition().GetVector());
		FRotator Rotator = FRotator(FConversions::ROSToU(ConstraintDescription.GetPose().GetOrientation().GetQuat()));


		//Execute spawning on GameThreade
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			bSuccess = FConstraintSpawner::SpawnPhysicsConstraintActor(World, ConstraintDescription.GetId(), Details, Location, Rotator);
		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

		bAllSuccessfull.AtomicSet(bAllSuccessfull ? bSuccess : bAllSuccessfull);
		if (!bSuccess)
		{
			FailedConstraintIds.Add(ConstraintDescription.GetId());
		}
	}

	// Set all relations on GameThread
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{

		TArray<AActor*> Actors;
		for (auto RelationDescription : SpawnSemanticMapRequest->GetRelations())
		{
			bSuccess = false;
			UGameplayStatics::GetAllActorsWithTag(World, FName(*RelationDescription.GetChildId()), Actors);
			AActor* Child = Actors.Pop();
			UGameplayStatics::GetAllActorsWithTag(World, FName(*RelationDescription.GetParentId()), Actors);
			AActor* Parent = Actors.Pop();

			if (Child && Parent)
			{
				//Actors were found and will be attached
				bSuccess = FAssetModifier::AttachToParent(Parent, Child);
			}
			
			if (!Child)
			{
				UE_LOG(LogTemp, Error, TEXT("[%s]: Actor with id:\"%s\" does not exist."), *FString(__FUNCTION__), *RelationDescription.GetChildId());
			}
			 
			if (!Parent)
			{
				UE_LOG(LogTemp, Error, TEXT("[%s]: Actor with id:\"%s\" does not exist."), *FString(__FUNCTION__), *RelationDescription.GetParentId());
			}

			bAllSuccessfull.AtomicSet(bAllSuccessfull ? bSuccess : bAllSuccessfull);

			if (!bSuccess)
			{
				FailedRelations.Add(RelationDescription);
			}			
		}
	}, TStatId(), nullptr, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	// Service response
	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSSpawnSemanticMapSrv::Response(bAllSuccessfull, FailedModelIds, FailedConstraintIds, FailedRelations));

}

