#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotDescription/RDJointAxis.h"
#include "RDJoint.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "RobotDescription", DefaultToInstanced, hidecategories = Object, editinlinenew)
class UROBOSIM_API URDJoint : public UObject
{
	GENERATED_BODY()

public:
	URDJoint();

	UPROPERTY(EditAnywhere, Category = "Joint")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Joint")
	FString Type;

	UPROPERTY(EditAnywhere, Category = "Joint")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Joint")
	FString Parent;

	UPROPERTY(EditAnywhere, Category = "Joint")
	FString Child;

	UPROPERTY(EditAnywhere, Category = "Joint")
	URDJointAxis* Axis;
};
