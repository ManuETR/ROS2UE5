#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotDescription/RDGeometry.h"
#include "RDVisual.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "RobotDescription")
class UROBOSIM_API URDVisual : public UObject
{
	GENERATED_BODY()

public:
	URDVisual();

	UPROPERTY(EditAnywhere, Category = "Visual")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Visual")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Visual|Geometry")
	URDGeometry* Geometry;
};
