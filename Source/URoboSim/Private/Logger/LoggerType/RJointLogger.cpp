#include "Logger/LoggerType/RJointLogger.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointLogger, Log, All);

URJointLogger::URJointLogger()
{
  bLogAllJoints = false;
}

void URJointLogger::SetLoggerParameters(URLoggerParameter *&LoggerParameters)
{
  URJointLoggerParameter *JointLoggerParameters = Cast<URJointLoggerParameter>(LoggerParameters);
  if (JointLoggerParameters)
  {
    bLogAllJoints = JointLoggerParameters->bLogAllJoints;
  }
}

void URJointLogger::Init()
{
  Super::Init();

  // bPublishResult = false;
  if (GetOwner())
  {
    if (bLogAllJoints)
    {
      for (URJoint *&Joint : GetOwner()->GetJoints())
      {
        JointNames.Add(Joint->GetName());
      }
    }
  }
  else
  {
    UE_LOG(LogRJointLogger, Error, TEXT("%s is not attached to ARModel"), *GetName())
  }
}

FString URJointLogger::Tick(const float &InDeltaTime)
{
  FDateTime CurrentTime = FDateTime::Now();

  // Calculate the time elapsed since the last log
  FTimespan ElapsedTime = CurrentTime - LastLogTime;

  // Check if at least one second has passed
  if (ElapsedTime.GetTotalSeconds() < LogDelay / 1000) {
    return TEXT("");
  }
  else {
    LastLogTime = CurrentTime;
  }

  if (GetOwner())
  {
    for (const FString& JointName : JointNames)
    {
      if (URJoint* Joint = GetOwner()->GetJoint(JointName))
      {
        
      }
      else
      {
        UE_LOG(LogRJointLogger, Error, TEXT("%s of %s not found"), *JointName, *GetOwner()->GetName())
      }
    }
  }
  else
  {
    UE_LOG(LogRJointLogger, Error, TEXT("Owner of %s not found"), *GetName())
  }

  return TEXT("TEST");
}
