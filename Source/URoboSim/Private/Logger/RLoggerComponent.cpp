#include "Logger/RLoggerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRLoggerComponent, Log, All);

URLoggerComponent::URLoggerComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void URLoggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
  for (URLogger *&Logger : Loggers)
  {
    TSharedPtr<FJsonObject> JsonObject = Logger->GetData(DeltaTime);
    if (JsonObject.IsValid()) {
      Log(JsonObject);
    }
  }
}

void URLoggerComponent::Init()
{
  if (ARModel *Owner = Cast<ARModel>(GetOwner()))
  {
    for (URLogger *&Logger : Loggers)
    {
      Logger->Init();
    }
  }
  else
  {
    UE_LOG(LogRLoggerComponent, Error, TEXT("Owner of %s is not ARModel"), *GetName())
  }
}

URLogger *URLoggerComponent::GetLogger(const FString &LoggerName) const
{
  URLogger *const *LoggerPtr = Loggers.FindByPredicate([&](URLogger *Logger){ return Logger->GetName().Contains(LoggerName); });
  if (LoggerPtr)
  {
    return *LoggerPtr;
  }
  else
  {
    UE_LOG(LogRLoggerComponent, Error, TEXT("%s of %s not found"), *LoggerName, *GetName())
    return nullptr;
  }
}

void URLoggerComponent::Log(TSharedPtr<FJsonObject> JsonObject) {
  if (Sink == UDataSink::Seq) {
    USeqLog::Get()->Send(JsonObject);
  }
}