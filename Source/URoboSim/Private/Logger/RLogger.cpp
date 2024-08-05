#include "Logger/RLogger.h"
#include "Logger/RLoggerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRLogger, Log, All);

void URLogger::Init()
{
  LastLogTime = FDateTime::Now();
  SetOwner();
}

void URLogger::SetOwner()
{
  UE_LOG(LogRLogger, Log, TEXT("Initialize %s"), *GetName())
  if (!Owner)
  {
    /*
    if (Cast<ARModel>(GetOuter()))
    {
      Owner = Cast<ARModel>(GetOuter());
    }
    else 
    */
    if (Cast<URLoggerComponent>(GetOuter()) && Cast<ARModel>(Cast<URLoggerComponent>(GetOuter())->GetOwner()))
    {
      Owner = Cast<ARModel>(Cast<URLoggerComponent>(GetOuter())->GetOwner());
    }
  }
  if (!Owner)
  {
    UE_LOG(LogRLogger, Error, TEXT("Owner of %s not found, Outer is %s"), *GetName(), *GetOuter()->GetName());
  }
}


TSharedPtr<FJsonObject> URLogger::GetData(const float& InDeltaTime)
{
  return nullptr;
}