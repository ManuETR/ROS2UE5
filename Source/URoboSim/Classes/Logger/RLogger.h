
#pragma once
#include "Physics/RModel.h"
#include "Misc/DateTime.h"
#include "Json.h"
#include "JsonUtilities.h"
// clang-format off
#include "RLogger.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URLoggerParameter : public UObject
{
  GENERATED_BODY()

};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URLogger : public UObject
{
  GENERATED_BODY()

public:
  virtual TSharedPtr<FJsonObject> GetData(const float& InDeltaTime);
  virtual void Init();
  virtual void SetLoggerParameters(URLoggerParameter *&LoggerParameters) {}

  URLogger(){LoggerName = GetName();};

  ARModel *GetOwner() const { return Owner; }

  void SetOwner(UObject *InOwner) { Owner = Cast<ARModel>(InOwner); }
  void SetOwner();

  bool bActive = true;

  UPROPERTY(VisibleAnywhere)
  FString LoggerName;

  UPROPERTY(EditAnywhere)
  int LogDelay = 1000;

protected:
  ARModel *Owner;
  FDateTime LastLogTime;
};
