#pragma once

#include "Logger/RLogger.h"
#include "RUtilityClasses.h"
// clang-format off
#include "RJointLogger.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointLoggerParameter : public URLoggerParameter
{
  GENERATED_BODY()

public:
  URJointLoggerParameter()
  {
    bLogAllJoints = false;
  }

  UPROPERTY(EditAnywhere)
  bool bLogAllJoints;

  UPROPERTY(EditAnywhere)
  TArray<FString> LogJoints;
};

UCLASS()
class UROBOSIM_API URJointLogger : public URLogger
{
  GENERATED_BODY()

public:
  URJointLogger();

public:
  virtual void Init() override;
  virtual TSharedPtr<FJsonObject> GetData(const float& InDeltaTime) override;
  virtual void SetLoggerParameters(URLoggerParameter *&LoggerParameters) override;

protected:
  UPROPERTY(EditAnywhere)
  bool bLogAllJoints;

  UPROPERTY(EditAnywhere)
  TArray<FString> JointNames;

};
