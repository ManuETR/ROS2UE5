#pragma once
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "AssetSpawner.generated.h"



USTRUCT()
struct UWORLDCONTROL_API FAssetSpawner
{
	GENERATED_USTRUCT_BODY()

public:

	struct FPhysicsProperties
	{
		EComponentMobility::Type Mobility;
		bool bSimulatePhysics;
		bool bGravity;
		bool bGenerateOverlapEvents;
		float Mass;
	};

	struct FTag
	{
		FString TagType;
		FString Key;
		FString Value;
	};

	struct FSpawnAssetParams
	{
		FString Id;
		FString Name;
		FVector Location;
		FRotator Rotator;
		TArray<FTag> Tags;
		FPhysicsProperties PhysicsProperties;
		FString StartDir;
		FString ActorLabel;
		FString OverrideName;
		TArray<FString> MaterialNames;
		TArray<FString> MaterialPaths;
		FString ParentId;
        	float Height;
        	float Width;
        	float Depth;
		bool bSpawnCollisionCheck;
	};

    static bool SpawnAsset(UWorld* World, FSpawnAssetParams Params, FString &FinalActorName, FString &ErrType);
    static bool SpawnProMeshAsset(UWorld* World,FSpawnAssetParams Params,FString &FinalActorName, FString &ErrType);

};


