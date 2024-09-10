#include "ConfigHandler.h"
#include "Editor.h"
#include "Engine/World.h"
#include "FileHelpers.h"
#include "URoboSim/Classes/Physics/RModel.h"
#include "URoboSim/Classes/Factory/RModelBuilder.h"
#include "URoboSim/Classes/SDF/SDFDataAsset.h"
#include "URoboSim/Classes/ROSCommunication/Subscriber/JointStateSubscriber.h"
#include "URoboSim/Classes/ROSCommunication/Subscriber/RSubscriber.h"
#include "URoboSim/Classes/ROSCommunication/RROSCommunicationComponent.h"
#include "URoboSim/Classes/Controller/RControllerComponent.h"
#include "URoboSim/Classes/Controller/ControllerType/JointController/RJointController.h"
#include "URoboSim/Classes/Logger/RLoggerComponent.h"

#define LOCTEXT_NAMESPACE "FConfigHandlerModule"

void FConfigHandlerModule::StartupModule()
{
  FEditorDelegates::OnEditorBoot.AddRaw(this, &FConfigHandlerModule::OnEditorInit);

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FConfigHandlerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


FString FConfigHandlerModule::CommandLineArgValue()
{
  FString s;
  UE_LOG(LogTemp, Log, TEXT("[Config] CommandLineArg %s"), FCommandLine::Get());
  if (FParse::Value(FCommandLine::Get(), TEXT("-config="), s)) {
    UE_LOG(LogTemp, Log, TEXT("[Config] CommandLineArg %s"), *s);
    return s;
  }

  return TEXT("");
}

void FConfigHandlerModule::LoadROSConfig(TSharedPtr<FJsonObject> ROSConfig) {
  TSharedPtr<FJsonObject> ROSBridgeConfig = ROSConfig->GetObjectField(TEXT("bridge"));

  if (ROSBridgeConfig.IsValid()) {
    if (ROSBridgeConfig->HasField(TEXT("ip"))) {
      // ROSBridgeServerHost = ROSBridgeConfig->GetStringField(TEXT("ip"));
      // UE_LOG(LogTemp, Log, TEXT("[Config] Setting rosbridge host to %s"), *ROSBridgeServerHost);
    }

    if (ROSBridgeConfig->HasField(TEXT("port"))) {
      // ROSBridgeServerPort = ROSBridgeConfig->GetNumberField(TEXT("port"));
      // UE_LOG(LogTemp, Log, TEXT("[Config] Setting rosbridge port to %d"), ROSBridgeServerPort);
    }
  }

}

void FConfigHandlerModule::LoadEnvironmentConfig(TSharedPtr<FJsonObject> EnvironmentConfig) {
  if (EnvironmentConfig->HasField(TEXT("map"))) {
    FString Map = "/Game/" + EnvironmentConfig->GetStringField(TEXT("map"));

    UE_LOG(LogTemp, Log, TEXT("[Config] Loading world %s"), *Map);
    World = UEditorLoadingAndSavingUtils::LoadMap(Map);
  }
}


void FConfigHandlerModule::LoadRobotsConfig(TArray<TSharedPtr<FJsonValue>> RobotsConfig) {
  for (TSharedPtr<FJsonValue> Config : RobotsConfig) {
    TSharedPtr<FJsonObject> RobotConfig = Config->AsObject();
    FVector Location(0, 0, 0);
    FRotator Rotation(0, 0, 0);

    if (RobotConfig->HasField("robot")) {
      FString Path = "/Game/" + RobotConfig->GetStringField("robot") + "." + RobotConfig->GetStringField("robot");
      USDFDataAsset* DataAsset = Cast<USDFDataAsset>(StaticLoadObject(USDFDataAsset::StaticClass(), nullptr, *Path));

      UE_LOG(LogTemp, Log, TEXT("[Config] Loading robot from %s"), *Path);

      if (RobotConfig->HasField("position")) {
        TArray<TSharedPtr<FJsonValue>> Position = RobotConfig->GetArrayField("position");
        Location = FVector(Position[0]->AsNumber(), Position[1]->AsNumber(), Position[2]->AsNumber());
      }

      URModelBuilder* ModelBuilder = NewObject<URModelBuilder>();
      FTransform Transform = FTransform(Rotation, Location);
      ARModel * NewRobot = World->SpawnActor<ARModel>(ARModel::StaticClass()->GetDefaultObject<ARModel>()->GetClass(), Location, Rotation, FActorSpawnParameters());
      ModelBuilder->Load(DataAsset->Models[0], NewRobot, Transform.GetLocation());
      //ARModel* SpawnedActor = World->SpawnActor<ARModel>(ARModel::StaticClass(), Location, Rotation, FActorSpawnParameters());

      if (RobotConfig->HasField("subscribers")) {
        for (TSharedPtr<FJsonValue> S : RobotConfig->GetArrayField("subscribers")) {
          TSharedPtr<FJsonObject> SubscriberConfig = S->AsObject();
          URROSCommunicationComponent * ROSCommunicationComponent = NewRobot->GetComponentByClass<URROSCommunicationComponent>();

          if (SubscriberConfig->HasField("type") && SubscriberConfig->HasField("topic")) {
            if (SubscriberConfig->GetStringField("type") == "JointStateSubscriber") {
              URSubscriber* ROSSubscriber = NewObject<URJointStateSubscriber>(NewRobot);
              ROSSubscriber->Topic = SubscriberConfig->GetStringField("topic");
              ROSCommunicationComponent->AddSubscriber(ROSSubscriber);
            }
          }
          else {
            UE_LOG(LogTemp, Error, TEXT("[Config] subscriber config wrong formatted"));
          }
        }
      }

      if (RobotConfig->HasField("controllers")) {
        for (TSharedPtr<FJsonValue> C : RobotConfig->GetArrayField("controllers")) {
          TSharedPtr<FJsonObject> ControllerConfig = C->AsObject();
          URControllerComponent* ControllerComponent = NewRobot->GetComponentByClass<URControllerComponent>();

          if (ControllerConfig->HasField("type") && ControllerConfig->HasField("mode")) {
            if (ControllerConfig->GetStringField("type") == "JointController") {
              URController* Controller = NewObject<URJointController>(NewRobot);
              URJointControllerParameter* Param = NewObject<URJointControllerParameter>();
              Param->Mode = ControllerConfig->GetStringField("mode") == "dynamic" ? UJointControllerMode::Dynamic : UJointControllerMode::Kinematic;
              URControllerParameter* P = Param;
              Controller->SetControllerParameters(P);
              ControllerComponent->AddController(Controller);
            }
          }
          else {
            UE_LOG(LogTemp, Error, TEXT("[Config] subscriber config wrong formatted"));
          }
        }
      }
      if (RobotConfig->HasField("tasks")) {
        // TODO
      }
    }
  }
}

void FConfigHandlerModule::LoadLoggingConfig(TSharedPtr<FJsonObject> LogginConfig) {
  if (LogginConfig->HasField("enable")) {
    // TODO enable logging
  }
}

void FConfigHandlerModule::OnEditorInit(double Duration) {
  UE_LOG(LogTemp, Log, TEXT("[Config] Start loading..."));
  LoadCustomConfig();
  UE_LOG(LogTemp, Log, TEXT("[Config] Finished"));
}

void FConfigHandlerModule::LoadCustomConfig()
{
  FString ConfigFilePath = CommandLineArgValue();
  if (ConfigFilePath.IsEmpty())
  {
    return;
  }

  FString ConfigFileContent;
  FFileHelper::LoadFileToString(ConfigFileContent, *ConfigFilePath);

  TSharedPtr<FJsonObject> JsonParsed;
  TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ConfigFileContent);

  if (FJsonSerializer::Deserialize(Reader, JsonParsed) && JsonParsed.IsValid())
  {
    for (const auto& Entry : JsonParsed->Values)
    {
      FString Key = Entry.Key;

      if (Key == TEXT("environment")) {
        LoadEnvironmentConfig(Entry.Value->AsObject());
      }
      else if (Key == TEXT("robots")) {
        LoadRobotsConfig(Entry.Value->AsArray());
      }
      else if (Key == TEXT("logging")) {
        LoadLoggingConfig(Entry.Value->AsObject());
      }
      else if (Key == TEXT("ros")) {
        LoadROSConfig(Entry.Value->AsObject());
      }
    }
  }

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FConfigHandlerModule, ConfigHandlers)