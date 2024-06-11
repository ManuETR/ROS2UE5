// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "KismetProceduralMeshLibrary.h"
//#include "ProceduralMeshComponent.h"
//#include "CustomMeshComponent.h"
#include "BoundingBox.generated.h"

UCLASS()
class UWORLDCONTROL_API ABoundingBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoundingBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    //UPROPERTY(EditAnywhere)
    //UProceduralMeshComponent* ProMesh;

    //UPROPERTY(EditAnywhere)
    //UProceduralMeshComponent* Front;


	
};
