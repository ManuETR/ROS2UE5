#pragma once
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "AssetHighlighter.generated.h"



USTRUCT()
struct UWORLDCONTROL_API FAssetHighlighter
{
	GENERATED_USTRUCT_BODY()

public:

	static bool Highlight(AActor* ActorToBeHighlighted, uint8 Color);

};


