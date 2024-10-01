#include "ConfigLoader.h"
#include "Editor.h"
#include "Engine/World.h"
#include "FileHelpers.h"
#include "URoboSim/Classes/Physics/RModel.h"
#include "URoboSim/Classes/Factory/RModelBuilder.h"
#include "URoboSim/Classes/RobotDescription/RDDataAsset.h"
#include "URoboSim/Classes/ROSCommunication/Subscriber/JointStateSubscriber.h"
#include "URoboSim/Classes/ROSCommunication/Subscriber/RSubscriber.h"
#include "URoboSim/Classes/ROSCommunication/RROSCommunicationComponent.h"
#include "URoboSim/Classes/Controller/RControllerComponent.h"
#include "URoboSim/Classes/Controller/ControllerType/JointController/RJointController.h"
#include "URoboSim/Classes/Logger/RLoggerComponent.h"

#define LOCTEXT_NAMESPACE "FConfigLoaderModule"

void FConfigLoaderModule::StartupModule()
{
  FEditorDelegates::OnEditorBoot.AddRaw(this, &FConfigLoaderModule::OnEditorInit);

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FConfigLoaderModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FConfigLoaderModule::StartSimulationInEditor()
{
  // Check if we are in the editor and not in standalone game mode
  if (GEditor && GEditor->IsPlayingSessionInEditor() == false)
  {
    // Log that we're starting play in the editor
    UE_LOG(LogTemp, Log, TEXT("Starting Play in Editor"));

    // Trigger the Play In Editor session
    FRequestPlaySessionParams PlaySessionParams;
    PlaySessionParams.DestinationSlateViewport = nullptr;   // Play in the current viewport
    PlaySessionParams.SessionDestination = EPlaySessionDestinationType::InProcess;  // Set it to PIE (Play In Editor)

    // Automatically uses the currently selected viewport in the editor
    PlaySessionParams.EditorPlaySettings = GetMutableDefault<ULevelEditorPlaySettings>();
    PlaySessionParams.EditorPlaySettings->LastExecutedPlayModeType = PlayMode_InViewPort;
    PlaySessionParams.StartLocation = FVector(100, -100, 80);
    PlaySessionParams.StartRotation = FRotator(-15, 135, 0);

    GEditor->RequestPlaySession(PlaySessionParams);
    FWorldDelegates::OnStartGameInstance.AddLambda([this](UGameInstance* GameInstance) {
      int TimeInSeconds = CommandLineArgMaxSimTime();
      UE_LOG(LogTemp, Warning, TEXT("Starting sim timer %d"), TimeInSeconds);
      GameInstance->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Stop, []() {
        UE_LOG(LogTemp, Warning, TEXT("Timer over!"));

        UE_LOG(LogTemp, Log, TEXT("Stopping Play In Editor session"));
        if (GEditor)
        {
          GEditor->RequestEndPlayMap();  // Stops the play session if it's still running.
        }

        FGenericPlatformMisc::RequestExit(false);

        }, TimeInSeconds, false);
    });
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Play In Editor is already running or we are not in the editor!"));
  }
}

bool FConfigLoaderModule::IsAutoStartEnabled()
{
  FString configValue;

  // Check if the -autostart argument is present
  if (FParse::Param(FCommandLine::Get(), TEXT("autostart"))) {
    UE_LOG(LogTemp, Log, TEXT("[Autostart] Autostart enabled"));
    return true;
  }

  UE_LOG(LogTemp, Log, TEXT("[Autostart] Autostart not found"));
  return false;
}

int FConfigLoaderModule::CommandLineArgMaxSimTime()
{
  FString s;
  if (FParse::Value(FCommandLine::Get(), TEXT("-maxSimTime="), s)) {
    UE_LOG(LogTemp, Log, TEXT("[Config] max sim time %s"), *s);
    return FCString::Atoi(*s);
  }

  return 10;
}

FString FConfigLoaderModule::CommandLineArgValue()
{
  FString s;
  UE_LOG(LogTemp, Log, TEXT("[Config] CommandLineArg %s"), FCommandLine::Get());
  if (FParse::Value(FCommandLine::Get(), TEXT("-config="), s)) {
    UE_LOG(LogTemp, Log, TEXT("[Config] CommandLineArg %s"), *s);
    return s;
  }

  return TEXT("");
}

/*
  Load the "ros" object from the json
  { "bridge": { "ip": string, "port" : number } }
*/ 
void FConfigLoaderModule::LoadROSConfig(TSharedPtr<FJsonObject> ROSConfig) {
  TSharedPtr<FJsonObject> ROSBridgeConfig = ROSConfig->GetObjectField(TEXT("bridge"));

  if (!World) {
    UE_LOG(LogTemp, Error, TEXT("[Config] World is null! Cannot load ROS config."));
    return;
  }

  URoboSimGameInstance* GameInstance = (URoboSimGameInstance*) World->GetGameInstance();

  if (!GameInstance) {
    UE_LOG(LogTemp, Error, TEXT("[Config] GameInstance is null or not of type URoboSimGameInstance!"));
    return;
  }

  if (ROSBridgeConfig.IsValid()) {
    if (ROSBridgeConfig->HasField(TEXT("ip"))) {
      GameInstance->ROSBridgeServerHost = ROSBridgeConfig->GetStringField(TEXT("ip"));
      UE_LOG(LogTemp, Log, TEXT("[Config] Setting rosbridge host to %s"), *GameInstance->ROSBridgeServerHost);
    }

    if (ROSBridgeConfig->HasField(TEXT("port"))) {
      GameInstance->ROSBridgeServerPort = ROSBridgeConfig->GetNumberField(TEXT("port"));
      UE_LOG(LogTemp, Log, TEXT("[Config] Setting rosbridge port to %d"), GameInstance->ROSBridgeServerPort);
    }
  }

}

/*
  Load the "environment" object from the json
  { "map": string }
*/
void FConfigLoaderModule::LoadEnvironmentConfig(TSharedPtr<FJsonObject> EnvironmentConfig) {
  if (EnvironmentConfig->HasField(TEXT("map"))) {
    FString Map = "/Game/" + EnvironmentConfig->GetStringField(TEXT("map"));

    UE_LOG(LogTemp, Log, TEXT("[Config] Loading world %s"), *Map);
    World = UEditorLoadingAndSavingUtils::LoadMap(Map);
  }
}


/*
  Load the "environment" object from the json
  { "robot": string, "position": array, "subscribers": array<Subscribers>, "controllers": array<Controller> }
*/
void FConfigLoaderModule::LoadRobotsConfig(TArray<TSharedPtr<FJsonValue>> RobotsConfig) {
  for (TSharedPtr<FJsonValue> Config : RobotsConfig) {
    TSharedPtr<FJsonObject> RobotConfig = Config->AsObject();
    FVector Location(0, 0, 0);
    FRotator Rotation(0, 0, 0);

    if (RobotConfig->HasField("robot")) {
      FString Path = "/Game/" + RobotConfig->GetStringField("robot") + "." + RobotConfig->GetStringField("robot");
      URDDataAsset* DataAsset = Cast<URDDataAsset>(StaticLoadObject(URDDataAsset::StaticClass(), nullptr, *Path));

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

/*
  Load the "logging" object from the json
  { "map": string }
*/
void FConfigLoaderModule::LoadLoggingConfig(TSharedPtr<FJsonObject> LogginConfig) {
  if (LogginConfig->HasField("enable")) {
    // TODO enable logging
  }
}

void FConfigLoaderModule::OnEditorInit(double Duration) {
  UE_LOG(LogTemp, Log, TEXT("[Config] Start loading..."));
  LoadCustomConfig();
  UE_LOG(LogTemp, Log, TEXT("[Config] Finished"));

  if (IsAutoStartEnabled())
  {
    StartSimulationInEditor();
  }
}

void FConfigLoaderModule::LoadCustomConfig()
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
	
IMPLEMENT_MODULE(FConfigLoaderModule, ConfigLoaders)