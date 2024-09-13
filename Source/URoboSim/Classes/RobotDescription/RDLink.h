#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotDescription/RDVisual.h"
#include "RobotDescription/RDCollision.h"
#include "RobotDescription/RDLinkInertial.h"
#include "RDLink.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "RobotDescription")
class UROBOSIM_API URDLink : public UObject
{
	GENERATED_BODY()

public:
	URDLink();

	UPROPERTY(EditAnywhere, Category = "Link")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Link")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Link")
	bool bSelfCollide;

	UPROPERTY(EditAnywhere, Category = "Link")
	bool bGravity;

	UPROPERTY(EditAnywhere, Category = "Link")
	URDLinkInertial* Inertial;

	UPROPERTY(EditAnywhere, Category = "Link")
	TArray<URDCollision*> Collisions;

	UPROPERTY(EditAnywhere, Category = "Link")
	TArray<URDVisual*> Visuals;
};
