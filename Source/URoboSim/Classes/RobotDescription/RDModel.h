#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotDescription/RDLink.h"
#include "RobotDescription/RDJoint.h"
#include "RobotDescription/RDPlugin.h"
#include "RDModel.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "RobotDescription")
class UROBOSIM_API URDModel : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Model")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Model")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Model")
	bool bStatic;

	UPROPERTY(EditAnywhere, Category = "Model")
	TArray<URDLink*> Links;

	UPROPERTY(EditAnywhere, Category = "Model")
	TArray<URDJoint*> Joints;

	UPROPERTY(EditAnywhere, Category = "Model")
	TArray<URDPlugin*> Plugins;
};
