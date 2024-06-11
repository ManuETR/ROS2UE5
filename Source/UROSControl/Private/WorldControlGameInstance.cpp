// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldControlGameInstance.h"
#include "ResetLevelServer.h"

UWorldControlGameInstance::UWorldControlGameInstance()
{
  Manager = CreateDefaultSubobject<URWCManager>(TEXT("RWCManager"));
}

void UWorldControlGameInstance::OnStart()
{
  Super::OnStart();

  if(ROSHandler.IsValid())
    {
      UWorld* World = GetWorld();
      if(World)
      {
	ROSHandler->AddServiceServer(MakeShareable<FROSResetLevelServer>(new FROSResetLevelServer(Namespace, TEXT("reset_level"),  this)));
        if(Manager)
          {
            Manager->Register(Namespace, World);
            Manager->ConnectToHandler(ROSHandler);
          }

      }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("World not ready"));
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("GameInstance: Handler not valid"));
    }
}
