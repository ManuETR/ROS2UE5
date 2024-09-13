#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotDescription/RDGeometry.h"
#include "RDCollision.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "RobotDescription")
class UROBOSIM_API URDCollision : public UObject
{
	GENERATED_BODY()

public:
	URDCollision();

	UPROPERTY(EditAnywhere, Category = "Collision")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Collision")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Collision|Geometry")
	URDGeometry* Geometry;
};
