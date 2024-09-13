#include "Factory/RModelFactory.h"
#include "Factory/RModelBuilder.h"
#include "Physics/RModel.h"
#include "Engine/World.h"
#include "Editor/EditorEngine.h"

URModelFactory::URModelFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    // AActor subclass this ActorFactory creates.
    NewActorClassName = FString("ARModel");
    NewActorClass = ARModel::StaticClass();
}

bool URModelFactory::CanCreateActorFrom(const FAssetData & AssetData, FText & OutErrorMsg)
{
    return AssetData.GetClass() != nullptr && AssetData.GetClass()->IsChildOf( URDDataAsset::StaticClass());
}

AActor* URModelFactory::GetDefaultActor(const FAssetData & AssetData)
{
    return NewActorClass->GetDefaultObject<ARModel>();
}

AActor* URModelFactory::SpawnActor(UObject* InAsset, ULevel* InLevel, const FTransform& InTransform, const FActorSpawnParameters& InSpawnParams)
{
    URDDataAsset* RDAsset = CastChecked<URDDataAsset>(InAsset);
    if (RDAsset)
    {
        ARModel* NewRobot = nullptr;
        for (URDModel* Model : RDAsset->Models)
        {
            AActor* DefaultActor = GetDefaultActor(FAssetData(InAsset));
            if (DefaultActor)
            {
                //FActorSpawnParameters SpawnInfo;
                //SpawnInfo.OverrideLevel = InLevel;
                //SpawnInfo.ObjectFlags = InObjectFlags;
                //SpawnInfo.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;

                //TODO fix name of spawned model
                //SpawnInfo.Name = FName(*Model->Name);
                //UE_LOG(LogTemp, Error, TEXT("Create Model %s"), *SpawnInfo.Name.ToString());

                // Creates RRobot Actor.

                URModelBuilder* ModelBuilder = NewObject<URModelBuilder>(this);
                NewRobot = (ARModel*)InLevel->OwningWorld->SpawnActor(DefaultActor->GetClass(), &InTransform, InSpawnParams);
                ModelBuilder->Load(Model, NewRobot, InTransform.GetLocation());

                PostSpawnActor(InAsset, NewRobot);
            }
        }
        return NewRobot;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Asset cast to URDDataAsset failed"));
    }

    // Creates RRobot Actor.
    UE_LOG(LogTemp, Warning, TEXT("No default Robot Actor available\n"));
    return nullptr;
}

void URModelFactory::PostSpawnActor( UObject* Asset, AActor* NewActor )
{
  URDDataAsset* RDAsset = CastChecked<URDDataAsset>(Asset);
  if(RDAsset)
    {
      FActorLabelUtilities::SetActorLabelUnique(NewActor, RDAsset->Models[0]->Name);
    }
}

void URModelFactory::PostCreateBlueprint( UObject* Asset, AActor* CDO )
{

}
