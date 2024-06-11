// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "SrvCallbacks/SpawnModelsServer.h"
#include "Engine/StaticMeshActor.h"
#include "HAL/FileManagerGeneric.h"
#include "Ids.h"
#include "Conversions.h"
#include "AssetSpawner.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSpawnModelServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSSpawnModelSrv::Request> Request =
		MakeShareable(new FROSSpawnModelSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSpawnModelServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSSpawnModelSrv::Request> SpawnMeshRequest =
		StaticCastSharedPtr<FROSSpawnModelSrv::Request>(Request);


	// Move data to spawner Struct
	FAssetSpawner::FSpawnAssetParams Params;
	Params.Id = SpawnMeshRequest->GetId().IsEmpty() ? FIds::NewGuidInBase64() : SpawnMeshRequest->GetId();
	Params.Name = SpawnMeshRequest->GetName();
	Params.Location = FConversions::ROSToU(SpawnMeshRequest->GetPose().GetPosition().GetVector());
	Params.Rotator = FRotator(FConversions::ROSToU(SpawnMeshRequest->GetPose().GetOrientation().GetQuat()));
	for (auto Tag : SpawnMeshRequest->GetTags())
	{
		FAssetSpawner::FTag SpawnerTag;
		SpawnerTag.TagType = Tag.GetType();
		SpawnerTag.Key = Tag.GetKey();
		SpawnerTag.Value = Tag.GetValue();
		Params.Tags.Add(SpawnerTag);
	}

	Params.PhysicsProperties.Mobility = SpawnMeshRequest->GetPhysicsProperties().GetMobility();
	Params.PhysicsProperties.bSimulatePhysics = SpawnMeshRequest->GetPhysicsProperties().IsSimulatePhysics();

	
	Params.PhysicsProperties.bGravity = SpawnMeshRequest->GetPhysicsProperties().GetGravity();
	Params.PhysicsProperties.bGenerateOverlapEvents = SpawnMeshRequest->GetPhysicsProperties().GetGenerateOverlapEvents();
	Params.PhysicsProperties.Mass = SpawnMeshRequest->GetPhysicsProperties().GetMass();
	Params.StartDir = SpawnMeshRequest->GetPath();
	Params.ActorLabel = SpawnMeshRequest->GetActorLabel();
	Params.OverrideName = SpawnMeshRequest->GetOverrideName();
	Params.MaterialNames = SpawnMeshRequest->GetMaterialNames();
	Params.MaterialPaths = SpawnMeshRequest->GetMaterialPaths();
	Params.ParentId = SpawnMeshRequest->GetParentId();
	Params.bSpawnCollisionCheck = SpawnMeshRequest->GetSpawnCollisionCheck();

	FString FinalActorName;
	FString ErrType;
	// Execute on game thread
	double start = FPlatformTime::Seconds();
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		ServiceSuccess = FAssetSpawner::SpawnAsset(World, Params, FinalActorName, ErrType);
	}, TStatId(), nullptr, ENamedThreads::GameThread);

	//wait code above to complete
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
	double end = FPlatformTime::Seconds();
	UE_LOG(LogTemp, Display, TEXT("SpawnModel executed in %f seconds."), end-start);
	UE_LOG(LogTemp, Display, TEXT("ErrType is: %s"), *ErrType);
	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSSpawnModelSrv::Response(Params.Id, FinalActorName, *ErrType, ServiceSuccess));
}

