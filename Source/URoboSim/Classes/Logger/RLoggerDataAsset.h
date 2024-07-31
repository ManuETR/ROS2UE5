#pragma once

#include "Engine/DataAsset.h"
#include "Logger/RLogger.h"
#include "Containers/Map.h"
// clang-format off
#include "RLoggerDataAsset.generated.h"
// clang-format on

USTRUCT()
struct FRLoggerConfiguration
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, Instanced)
  TMap<FString, URLoggerParameter*> LoggerParameters;
};

UCLASS(BlueprintType, Category = "Logger")
class UROBOSIM_API URLoggerDataAsset : public UDataAsset
{
  GENERATED_BODY()

public:
  // Names of the robots for which this configration asset is used
  UPROPERTY(EditAnywhere, Category = "Basic Information")
  TArray<FString> RobotNames;

  UPROPERTY(EditAnywhere, Category = "Basic Information")
  FRLoggerConfiguration LoggerConfiguration;
};