// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
#pragma once

#include "CoreMinimal.h"
#include "Physics/RStaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "RGraspComponent.generated.h"

class URTFPublisher;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UROBOSIM_API URGraspComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	URGraspComponent();

	virtual void BeginPlay() override;

	UPROPERTY()
	AStaticMeshActor* FixatedObject;


	UPROPERTY()
	bool bObjectGrasped;


protected:
	UPROPERTY(EditAnywhere)
	float GraspRadius = 10.f;

	UPrimitiveComponent* Finger1;
	UPrimitiveComponent* Finger2;

	bool bFingerReady1;
	bool bFingerReady2;

	UPROPERTY(EditAnywhere)
	FString FingerName1;

	UPROPERTY(EditAnywhere)
	FString FingerName2;

  UPhysicsConstraintComponent* Constraint1;
  UPhysicsConstraintComponent* Constraint2;
	// Function called when an item enters the fixation overlap area
	UFUNCTION()
	virtual void OnFixationGraspAreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Function called when an item leaves the fixation overlap area
	UFUNCTION()
	virtual void OnFixationGraspAreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPrimitiveComponent* GetComponent(FString ComponentName);


	// Fixate object to hand
	void GraspObject(AStaticMeshActor* InSMA);
	void ReleaseObject();

        UPROPERTY()
          bool bGraspObjectGravity;
};
