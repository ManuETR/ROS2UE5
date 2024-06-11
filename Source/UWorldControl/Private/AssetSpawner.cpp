#include "AssetSpawner.h"
#include "AssetModifier.h"
#include "Engine/StaticMeshActor.h"
#include "BoundingBox.h"
#if WITH_EDITOR
#include "Engine/EngineTypes.h"
#include "Editor.h"
#endif


bool FAssetSpawner::SpawnAsset(UWorld* World, const FSpawnAssetParams Params, FString &FinalActorName, FString &ErrType)
{
	//Check if World is avialable
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]: Couldn't find the World."), *FString(__FUNCTION__));
		return false;
	}

	FString Label = (Params.ActorLabel.IsEmpty() ? Params.Name : Params.ActorLabel);
#if WITH_EDITOR
	GEditor->BeginTransaction(FText::FromString(TEXT("Spawning: ")+ Label));
	World->Modify();
#endif

	//Setup SpawnParameters
	FActorSpawnParameters SpawnParams;
  #if !WITH_EDITOR
  // Set the ActorName explicitly if supplied for non-editor builds
  if( !Params.OverrideName.IsEmpty())
  {
    SpawnParams.Name = *Params.OverrideName;
  }
  #endif

	//Load Mesh and check if it succeded.
	UStaticMesh* Mesh = FAssetModifier::LoadMesh(Params.Name, Params.StartDir);
	if (!Mesh)
	{
        UE_LOG(LogTemp, Error, TEXT("[%s]: Could not find Mesh: %s."), *FString(__FUNCTION__), *Params.Name);
#if WITH_EDITOR
		GEditor->EndTransaction();
#endif
		return false;
	}

	AStaticMeshActor* SpawnedItem;

	//Check if Id is used already
  TArray<AActor*> Actors;
  UGameplayStatics::GetAllActorsWithTag(World, FName(*Params.Id), Actors);

	if (!Actors.IsValidIndex(0))
	{
		// SpawnCollission Testing
		TArray<FOverlapResult> Results;
		bool bIsBlocked = World->OverlapMultiByChannel(Results, Params.Location, Params.Rotator.Quaternion(), ECollisionChannel::ECC_PhysicsBody, FCollisionShape::MakeBox(Mesh->GetBoundingBox().GetExtent()));

		if (bIsBlocked && Params.bSpawnCollisionCheck)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s]: Spawn Location is obstructed for \"%s\""), *FString(__FUNCTION__), *Params.Id);
			ErrType = "2";
#if WITH_EDITOR
			GEditor->EndTransaction();
#endif
			return false;
		}

		//Actual Spawning MeshComponent
		SpawnedItem = World->SpawnActor<AStaticMeshActor>(Params.Location, Params.Rotator, SpawnParams);

		// Needs to be movable if the game is running.
		SpawnedItem->SetMobility(EComponentMobility::Movable);
		//Assigning the Mesh and Material to the Component
		SpawnedItem->GetStaticMeshComponent()->SetStaticMesh(Mesh);

		if (Params.MaterialPaths.Num())
		{
			for (int i = 0; i < Params.MaterialPaths.Num(); i++)
			{
				UMaterialInterface* Material = FAssetModifier::LoadMaterial(Params.MaterialNames[i], Params.MaterialPaths[i]);
				if (Material)
				{
					SpawnedItem->GetStaticMeshComponent()->SetMaterial(i, Material);
				}
			}
		}

#if WITH_EDITOR
		SpawnedItem->SetActorLabel(Label);
#endif

		FPhysicsProperties Properties = Params.PhysicsProperties;
		SpawnedItem->GetStaticMeshComponent()->SetSimulatePhysics(Properties.bSimulatePhysics);
		SpawnedItem->GetStaticMeshComponent()->SetGenerateOverlapEvents(Properties.bGenerateOverlapEvents);
		SpawnedItem->GetStaticMeshComponent()->SetEnableGravity(Properties.bGravity);
		SpawnedItem->GetStaticMeshComponent()->SetMassOverrideInKg(NAME_None, Properties.Mass);

		SpawnedItem->SetMobility(Properties.Mobility);

	}
	else
	{
		//ID is already taken
		UE_LOG(LogTemp, Error, TEXT("[%s]: Semlog id: \"%s\" is not unique, therefore nothing was spawned."), *FString(__FUNCTION__), *Params.Id);
		ErrType = "1";
	
#if WITH_EDITOR
	GEditor->EndTransaction();
#endif
		return false;
	}

	//Id tag to Actor
  SpawnedItem->Tags.Add(FName(*Params.Id));
#if WITH_EDITOR
	SpawnedItem->Modify();
	GEditor->EndTransaction();
#endif
	FinalActorName = SpawnedItem->GetName();

	return true;
}





bool FAssetSpawner::SpawnProMeshAsset(UWorld *World, FSpawnAssetParams Params, FString &FinalActorName, FString &ErrType)
{
    //Check if World is avialable
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s]: Couldn't find the World."), *FString(__FUNCTION__));
        return false;
    }
    ABoundingBox* SpawnedActor=NewObject<ABoundingBox>();
    //    //Code what to do :D
    TArray<FVector> Vertices; //Points
    TArray<int32> Triangles;
    TArray<FVector>Normals;
    TArray<FVector2D>UVs;
    float height=Params.Height;
    float width=Params.Width;
    float depth=Params.Depth;

    //Assumption --> Boundingboxes are always square
    //We want the Mesh starting point to be in the middel of the Object so we will -1/2*height width depth to each of it
    float h=-height/2;  //heigh emphasis helper
    float w=-width/2;   //width emphasis helper
    float d=-depth/2;   //depth emphasis helper


    Vertices.Add(FVector(d,w,h)); //FrontBottomRight 0                                          //From Rotation (0|0|0) FrontButtomLeft
    Vertices.Add(FVector(d,w+width,h)); //BackBottomRight 1                                       //From Rotation (0|0|0) FrontButtomRight
    Vertices.Add(FVector(d+depth,w,h)); //FrontBottomLeft 2                                       //From Rotation (0|0|0) BackButtomLeft
    Vertices.Add(FVector(d+depth,w+width,h)); //BackBottomLeft 3                                    //From Rotation (0|0|0) BackButtonRight

    Vertices.Add(FVector(d,w,h+height)); //FrontTopRight 4                                        //From Rotation (0|0|0) FrontTopLeft
    Vertices.Add(FVector(d,w+width,h+height)); //BackTopRight 5                                     //From Rotation (0|0|0) FrontTopRight
    Vertices.Add(FVector(d+depth,w,h+height)); //FrontTopLeft 6                                     //From Rotation (0|0|0) BackTopLeft
    Vertices.Add(FVector(d+depth,w+width,h+height)); //BackTopLeft 7                                  //From Rotation (0|0|0) BackTopRight

    //    //Bottom
    Triangles.Add(2);
    Triangles.Add(3);
    Triangles.Add(0);

    Triangles.Add(3);
    Triangles.Add(1);
    Triangles.Add(0);

    //Back
    Triangles.Add(5);
    Triangles.Add(1);
    Triangles.Add(3);

    Triangles.Add(3);
    Triangles.Add(7);
    Triangles.Add(5);

    //Left
    Triangles.Add(3);
    Triangles.Add(2);
    Triangles.Add(6);

    Triangles.Add(3);
    Triangles.Add(6);
    Triangles.Add(7);

    //Top
    Triangles.Add(6);
    Triangles.Add(4);
    Triangles.Add(7);

    Triangles.Add(7);
    Triangles.Add(4);
    Triangles.Add(5);

    //Front
    Triangles.Add(2);
    Triangles.Add(0);
    Triangles.Add(6);

    Triangles.Add(6);
    Triangles.Add(0);
    Triangles.Add(5);


    //Front
    TArray<FVector> FrontVertices; //Points
    TArray<int32> FrontTriangles;
    TArray<FVector>FrontNormals;
    TArray<FVector2D>FrontUVs;

    //
    //From Playerstart Camera Postion
    FrontVertices.Add(FVector(d,w,h)); //FrontBottomRight 0                                          //From Rotation (0|0|0) FrontButtomLeft
    FrontVertices.Add(FVector(d,w+width,h)); //BackBottomRight 1                                       //From Rotation (0|0|0) FrontButtomRight
    FrontVertices.Add(FVector(d,w,h+height)); //FrontTopRight 2                                        //From Rotation (0|0|0) FrontTopLeft
    FrontVertices.Add(FVector(d,w+width,h+height)); //BackTopRight 3                                     //From Rotation (0|0|0) FrontTopRight

    FrontTriangles.Add(0);
    FrontTriangles.Add(1);
    FrontTriangles.Add(2);

    FrontTriangles.Add(2);
    FrontTriangles.Add(1);
    FrontTriangles.Add(3);

    FrontUVs.Add(FVector2D(0,1));
    FrontUVs.Add(FVector2D(1,1));
    FrontUVs.Add(FVector2D(0,0));
    FrontUVs.Add(FVector2D(1,0));



    FVector SpawnLocation=Params.Location;
    FRotator SpawnRotation= Params.Rotator;
    FActorSpawnParameters SpawnParams;

    FString ActorLabel = Params.ActorLabel;

    //Check if Id is used already
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsWithTag(World, FName(*Params.Id), Actors);

    if (!Actors.IsValidIndex(0))
    {
        //Actual Spawning MeshComponent
        SpawnedActor = World->SpawnActor<ABoundingBox>(SpawnLocation, SpawnRotation, SpawnParams);

        //SpawnedActor->ProMesh->CreateMeshSection(1,Vertices,Triangles,Normals,UVs,TArray<FColor>(),TArray<FProcMeshTangent>(),true); //create Mesh? needs to be in Game Thread?
        //SpawnedActor->Front->CreateMeshSection(1,FrontVertices,FrontTriangles,FrontNormals,FrontUVs,TArray<FColor>(),TArray<FProcMeshTangent>(),true);
        //// Needs to be movable if the game is running.
        //SpawnedActor->ProMesh->SetMobility(EComponentMobility::Movable);
        ////Assigning the Mesh and Material to the Component
        //SpawnedActor->ProMesh->SetupAttachment(SpawnedActor->GetRootComponent());
        //UE_LOG(LogTemp, Warning, TEXT("[%s]: Attached to new RootComponent."), *FString(__FUNCTION__));
        //SpawnedActor->ProMesh->SetSimulatePhysics(Params.PhysicsProperties.bSimulatePhysics);
        //SpawnedActor->ProMesh->SetGenerateOverlapEvents(Params.PhysicsProperties.bGenerateOverlapEvents);
        //SpawnedActor->ProMesh->SetEnableGravity(Params.PhysicsProperties.bGravity);
        //SpawnedActor->ProMesh->SetMassOverrideInKg(NAME_None,Params.PhysicsProperties.Mass);

        //SpawnedActor->ProMesh->SetMobility(Params.PhysicsProperties.Mobility);
    }
    else
    {
        //ID is already taken
        UE_LOG(LogTemp, Error, TEXT("[%s]: Semlog id: \"%s\" is not unique, therefore nothing was spawned."), *FString(__FUNCTION__), *Params.Id);
        return false;
    }

    SpawnedActor->Tags.Add(FName(*Params.Id));
    //SpawnedItem->Modify();

    FinalActorName = SpawnedActor->GetName();

//    return SpawnedActor;
    return true;
}

