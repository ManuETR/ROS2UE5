#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RDPlugin.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "RobotDescription")
class UROBOSIM_API URDPlugin : public UObject
{
	GENERATED_BODY()

public:
	URDPlugin();

	UPROPERTY(EditAnywhere, Category = "Plugin")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Plugin")
	FString Joint;

	UPROPERTY(EditAnywhere, Category = "Plugin")
	FString MimicJoint;

	UPROPERTY(EditAnywhere, Category = "Plugin")
	float Multiplier;
};
