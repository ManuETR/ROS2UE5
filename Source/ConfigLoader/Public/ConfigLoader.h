#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "URoboSimGameInstance.h"

class FConfigLoaderModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
  FString CommandLineArgValue();
  UWorld* World;
  FTimerHandle TimerHandle_Stop;

  void LoadCustomConfig();
  void LoadROSConfig(TSharedPtr<FJsonObject> ROSConfig);
  void LoadEnvironmentConfig(TSharedPtr<FJsonObject> EnvironmentConfig);
  void LoadRobotsConfig(TArray<TSharedPtr<FJsonValue>> RobotsConfig);
  void LoadLoggingConfig(TSharedPtr<FJsonObject> LoggingConfig);
  void OnEditorInit(double);
  void StartSimulationInEditor();
  bool IsAutoStartEnabled();
  int CommandLineArgMaxSimTime();
};
