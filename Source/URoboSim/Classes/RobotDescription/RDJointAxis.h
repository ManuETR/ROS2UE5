#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RDJointAxis.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "RobotDescription", DefaultToInstanced, hidecategories = Object, editinlinenew)
class UROBOSIM_API URDJointAxis : public UObject
{
	GENERATED_BODY()

public:

	URDJointAxis();

	UPROPERTY(EditAnywhere, Category = "Axis")
	FVector Xyz;

	UPROPERTY(EditAnywhere, Category = "Axis")
	bool bUseParentModelFrame;

	UPROPERTY(EditAnywhere, Category = "Axis|Limit")
	float Lower;

	UPROPERTY(EditAnywhere, Category = "Axis|Limit")
	float Upper;

	UPROPERTY(EditAnywhere, Category = "Axis|Limit")
	float Effort;

	UPROPERTY(EditAnywhere, Category = "Axis|Limit")
	float Velocity;

	void SetLowerLimitFrom(float LowerLimit);
	void SetUpperLimitFrom(float UpperLimit);
};
