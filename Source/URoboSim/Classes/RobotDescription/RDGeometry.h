#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "UObject/NoExportTypes.h"
#include "RDGeometry.generated.h"

/**
*
*/
UENUM(BlueprintType, Category = "RobotDescription")
enum class ERDGeometryType : uint8
{
	None			UMETA(DisplayName = "None"),
	Mesh			UMETA(DisplayName = "Mesh"),
	Box				UMETA(DisplayName = "Box"),
	Cylinder		UMETA(DisplayName = "Cylinder"),
	Sphere			UMETA(DisplayName = "Sphere"),
};

/**
 *
 */
UCLASS(BlueprintType, Category = "RobotDescription")
class UROBOSIM_API URDGeometry : public UObject
{
	GENERATED_BODY()

public:
	URDGeometry();

	UPROPERTY(EditAnywhere, Category = "Geometry")
	ERDGeometryType Type;

	UPROPERTY(EditAnywhere, Category = "Geometry|Mesh")
	FString Uri;

	UPROPERTY(EditAnywhere, Category = "Geometry|Mesh")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, Category = "Geometry|Box")
	FVector Size;

	UPROPERTY(EditAnywhere, Category = "Geometry|Cylinder or Sphere")
	float Radius;

	UPROPERTY(EditAnywhere, Category = "Geometry|Cylinder")
	float Length;
};
