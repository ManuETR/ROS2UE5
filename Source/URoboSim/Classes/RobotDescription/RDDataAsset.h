#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotDescription/RDModel.h"
#include "RDDataAsset.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "RobotDescription")
class UROBOSIM_API URDDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// Version
	UPROPERTY(EditAnywhere, Category = "RobotDescription")
	FString Version;

	// Array of models
	UPROPERTY(EditAnywhere, Category = "RobotDescription")
	TArray<URDModel*> Models;
};
