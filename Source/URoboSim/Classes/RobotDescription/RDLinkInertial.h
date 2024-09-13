#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RDLinkInertial.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "RobotDescription")
class UROBOSIM_API URDLinkInertial : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Link|Inertial")
	float Mass;

	UPROPERTY(EditAnywhere, Category = "Link|Inertial")
	FTransform Pose;
};
