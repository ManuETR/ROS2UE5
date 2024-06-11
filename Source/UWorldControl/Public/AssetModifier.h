#pragma once
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "AssetModifier.generated.h"



USTRUCT()
struct UWORLDCONTROL_API FAssetModifier
{
	GENERATED_USTRUCT_BODY()

public:
	static bool AttachToParent(AActor* Parent, AActor* Child);
	static bool ChangeVisual(UStaticMeshComponent* MeshComponent, TArray<FString> MaterialNames, TArray<FString> MaterialPaths);
	static bool RemoveAsset(UWorld* World, FString Id);
	static bool Relocate(AActor* Actor, FVector Location, FRotator Rotator);
	static bool ChangePhysicsProperties(UStaticMeshComponent* MeshComponent, EComponentMobility::Type Mobility, bool bSimulatedPhysics, bool bGereateOverlapEvents, bool bGravity, float Mass);
	
	static UStaticMesh* LoadMesh(FString Name, FString StartDir);
	static UMaterialInterface* LoadMaterial(FString Name, FString StartDir);
	
	//Constraint Modification
	/*static void SetupProfileInstance(FConstraintProfileProperties& ProfileInstance, world_control_msgs::PhysicsConstraintDetails Details);
	static void SetupAngularLimits(FConstraintInstance& Instance, world_control_msgs::PhysicsConstraintDetails Details);
	static void SetupLinearLimits(FConstraintInstance& Instance, world_control_msgs::PhysicsConstraintDetails Details);
	*/
};


