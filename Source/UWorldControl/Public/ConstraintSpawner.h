#pragma once
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "PhysicsEngine/ConstraintInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ConstraintSpawner.generated.h"


USTRUCT()
struct UWORLDCONTROL_API FConstraintSpawner
{
	GENERATED_USTRUCT_BODY()

public:
	struct FLinearLimits
	{
		uint8 XMotion;
		uint8 YMotion;
		uint8 ZMotion;
		float Limit;
		bool UseAdvanced;
		bool SoftConstraint;
		float Stiffness;
		float Damping;
	};

	struct FAngularLimits
	{
		uint8 Swing1Motion;
		uint8 Swing2Motion;
		uint8 TwistMotion;
		float Swing1LimitAngle;
		float Swing2LimitAngle;
		float TwistLimitAngle;
		FVector AngularRoationOffset;
		bool UseAdvanced;
		bool SwingSoftConstraint;
		float SwingStiffness;
		float SwingDamping;
		bool TwistSoftConstraint;
		float TwistStiffness;
		float TwistDamping;
	};

	struct FPhysicsConstraintDetails
	{
		FString IdFirstModel;
		FString IdSecondModel;
		bool DisableCollision;
		bool EnableProjection;
		float ProjectionLinearTolerance;
		float ProjectionAngularTolerance;
		bool ParentDominates;
		FLinearLimits LinearLimits;
		FAngularLimits AngularLimits;
	};

	static bool SpawnPhysicsConstraintActor(UWorld* World, FString Id, FPhysicsConstraintDetails Details, FVector Location, FRotator Rotator);

	//Constraint Modification
	static void SetupProfileInstance(FConstraintProfileProperties& ProfileInstance, FPhysicsConstraintDetails Details);
	static void SetupAngularLimits(FConstraintInstance& Instance, FAngularLimits AngLimit);
	static void SetupLinearLimits(FConstraintInstance& Instance, FLinearLimits LinLimit);

};


