#include "AssetHighlighter.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

bool FAssetHighlighter::Highlight(AActor* ActorToBeHighlighted, uint8 Color)
{
	if (ActorToBeHighlighted)
	{
#if WITH_EDITOR
		GEditor->BeginTransaction(FText::FromString(TEXT("Highlight: ") + ActorToBeHighlighted->GetActorLabel()));
#endif
		TArray<UStaticMeshComponent*> Components;
		ActorToBeHighlighted->GetComponents<UStaticMeshComponent>(Components);
		for (auto Component : Components)
		{
#if WITH_EDITOR
			Component->Modify();
#endif

			if (Color == 0)
			{
				Component->SetRenderCustomDepth(false);
			}
			else
			{
				Component->CustomDepthStencilValue = Color;
				Component->SetRenderCustomDepth(true);
			}
		}
#if WITH_EDITOR
		ActorToBeHighlighted->PostEditChange();
		ActorToBeHighlighted->Modify();
		GEditor->EndTransaction();
#endif
		return true;
	}
	else
	{

		return false;
	}
}
