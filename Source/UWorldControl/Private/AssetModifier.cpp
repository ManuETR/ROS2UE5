#include "AssetModifier.h"
#if ENGINE_MINOR_VERSION > 23 || ENGINE_MAJOR_VERSION >4
#include "HAL/FileManagerGeneric.h"
#else
#include "FileManagerGeneric.h"
#endif
#if WITH_EDITOR
#include "Editor.h"
#endif

bool FAssetModifier::RemoveAsset(UWorld* World, FString Id)
{

	//Get Actor with ID
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(World, FName(*Id), Actors);

	for (auto ActorToBeRemoved : Actors)
	{
#if WITH_EDITOR
		GEditor->BeginTransaction(FText::FromString(TEXT("Destroing: ")
			+ ActorToBeRemoved->GetActorLabel()));
#endif
		bool bSuccess = ActorToBeRemoved->Destroy();
#if WITH_EDITOR
		GEditor->EndTransaction();
#endif
		return bSuccess;
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s]: Actor with id:\"%s\" does not exist and can therefore not be removed."), *FString(__FUNCTION__), *Id);



	return false;
}

bool FAssetModifier::Relocate(AActor* Actor, FVector Location, FRotator Rotator)
{
#if WITH_EDITOR
	GEditor->BeginTransaction(FText::FromString(TEXT("Relocating: ")
		+ Actor->GetActorLabel()));
#endif
	bool bSuccess = Actor->TeleportTo(Location, Rotator);
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]: Could not set %s to locaiton: %s, with Rotation: %s"), *FString(__FUNCTION__),
			*Actor->GetName(), *Location.ToString(), *Rotator.ToString());
	}
	else
	{
		Actor->Modify();
	}
#if WITH_EDITOR
	GEditor->EndTransaction();
#endif

	return bSuccess;
}

bool FAssetModifier::ChangePhysicsProperties(UStaticMeshComponent* MeshComponent,
	EComponentMobility::Type Mobility, bool bSimulatedPhysics,
	bool bGereateOverlapEvents, bool bGravity, float Mass)
{
	if (!MeshComponent) return false;

#if WITH_EDITOR
	GEditor->BeginTransaction(FText::FromString(TEXT("Changing Phyisics on: ")
		+ MeshComponent->GetOwner()->GetActorLabel()));
#endif

	MeshComponent->SetMobility(Mobility);
	MeshComponent->SetSimulatePhysics(bSimulatedPhysics);
	MeshComponent->SetGenerateOverlapEvents(bGereateOverlapEvents);
	MeshComponent->SetEnableGravity(bGravity);
	MeshComponent->SetMassOverrideInKg(NAME_None, Mass);

#if WITH_EDITOR
	MeshComponent->Modify();
	GEditor->EndTransaction();
#endif

	return true;
}

bool FAssetModifier::ChangeVisual(UStaticMeshComponent* MeshComponent, TArray<FString> MaterialNames, TArray<FString> MaterialPaths)
{
#if WITH_EDITOR
	GEditor->BeginTransaction(FText::FromString(TEXT("Changing Visual on: ")
		+ MeshComponent->GetOwner()->GetActorLabel()));
#endif
	bool bSuccess = false;

	if (MaterialPaths.Num() > 0)
	{
		for (int i = 0; i < MaterialPaths.Num(); i++)
		{
			//Try to load Material
			UMaterialInterface* Material = Cast<UMaterialInterface>(
				StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *MaterialPaths[i]));
			if (Material)
			{
				MeshComponent->SetMaterial(i, Material);
				bSuccess = true;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[%s]: Path %s does not point to a Material"), *FString(__FUNCTION__), *MaterialPaths[i]);
			}
		}

	}
	else
	{
		for (int i = 0; i < MaterialNames.Num(); i++)
		{
			UMaterialInterface* Material = nullptr;
			Material = LoadMaterial(*MaterialNames[i], TEXT(""));

			if (Material)
			{
				MeshComponent->SetMaterial(i, Material);
				bSuccess = true;
			}

		}
	}
#if WITH_EDITOR
	if (bSuccess)
		MeshComponent->Modify();
	GEditor->EndTransaction();
#endif
	return bSuccess;
}

bool FAssetModifier::AttachToParent(AActor* Parent, AActor* Child)
{


	if (Parent && Child)
	{
#if WITH_EDITOR
		GEditor->BeginTransaction(FText::FromString(TEXT("Attaching: ")
			+ Child->GetActorLabel() + TEXT(" to: ")
			+ Parent->GetActorLabel()));
#endif
		Child->AttachToActor(Parent, FAttachmentTransformRules::KeepWorldTransform);
#if WITH_EDITOR
		Child->Modify();
		Parent->Modify();
		GEditor->EndTransaction();
#endif
		return true;
	}
	return false;
}


UStaticMesh* FAssetModifier::LoadMesh(FString Name, FString StartDir)
{
	UStaticMesh* Mesh = nullptr;
	//Look for file Recursively

	FString Filename = Name.StartsWith(TEXT("SM_")) ? TEXT("") : TEXT("SM_");
	Filename += Name;
	Filename += Name.EndsWith(TEXT(".uasset")) ? TEXT("") : TEXT(".uasset");
    UE_LOG(LogTemp, Warning, TEXT("[%s]: SpawnModel Name %s"), *FString(__FUNCTION__),*Name);

	TArray<FString> FileLocations;
    FFileManagerGeneric Fm;
	Fm.FindFilesRecursive(FileLocations, *FPaths::ProjectContentDir().Append(StartDir), *Filename, true, false, true);

	if (FileLocations.Num() == 0)
	{
		//Try again with whole ContentDir
		Fm.FindFilesRecursive(FileLocations, *FPaths::ProjectContentDir(), *Filename, true, false, true);
	}

	for (auto Loc : FileLocations)
	{
		//Try all found files until one works.
		if (Mesh == nullptr)
		{
			Loc.RemoveFromStart(FPaths::ProjectContentDir());
			int Last;
			Loc.FindLastChar('.', Last);
			Loc.RemoveAt(Last, Loc.Len() - Last);

			FString FoundPath = "StaticMesh'/Game/" + Loc + ".SM_" + Name + "'";
            Mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *FoundPath));
		}
	}

	return Mesh;
}

UMaterialInterface* FAssetModifier::LoadMaterial(FString Name, FString StartDir)
{
	UMaterialInterface* Material = nullptr;
	FString Filename;
	if (Name.StartsWith(TEXT("M_")))
		Filename = Name + TEXT(".uasset");
	else
		Filename = TEXT("M_") + Name + TEXT(".uasset");

	TArray<FString> FileLocations;
	FFileManagerGeneric Fm;
	Fm.FindFilesRecursive(FileLocations, *FPaths::ProjectContentDir().Append(StartDir), *Filename, true, false, true);

	if (FileLocations.Num() == 0)
	{
		//Try again with whole ContentDir
		Fm.FindFilesRecursive(FileLocations, *FPaths::ProjectContentDir(), *Filename, true, false, true);
	}

	for (auto Loc : FileLocations)
	{
		//Try all found files until one works.
		if (Material == nullptr)
		{
			Loc.RemoveFromStart(FPaths::ProjectContentDir());
			int Last;
			Loc.FindLastChar('.', Last);
			Loc.RemoveAt(Last, Loc.Len() - Last);

			FString FoundPath = "StaticMesh'/Game" + Loc + ".M_" + Name + "'";
			Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *FoundPath));
		}
	}

	return Material;

}
