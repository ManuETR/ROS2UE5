#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "URoboSim/Public/URoboSimGameInstance.h"

class FConfigHandlerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
  FString CommandLineArgValue();
  UWorld* World;
  URoboSimGameInstance* GameInstance;

  void LoadCustomConfig();
  void LoadROSConfig(TSharedPtr<FJsonObject> ROSConfig);
  void LoadEnvironmentConfig(TSharedPtr<FJsonObject> EnvironmentConfig);
  void LoadRobotsConfig(TArray<TSharedPtr<FJsonValue>> RobotsConfig);
  void LoadLoggingConfig(TSharedPtr<FJsonObject> LoggingConfig);
  void OnEditorInit(double);
};
