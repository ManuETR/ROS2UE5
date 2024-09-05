#include "ROSCommunication/Action/Server/ActionCancelGoalService.h"
#include "actionlib_msgs/GoalID.h"

DEFINE_LOG_CATEGORY_STATIC(LogRActionCancelGoalService, Log, All)

URActionCancelGoalService::URActionCancelGoalService()
{
  MessageType = TEXT("action_msgs/srv/CancelGoal");
}

void URActionCancelGoalService::CreateSubscriber()
{
  if (GetOwner())
  {
    Subscriber = MakeShareable<FActionCancelGoalServiceCallback>(
        new FActionCancelGoalServiceCallback(Topic, MessageType, Controller));
  }
}

FActionCancelGoalServiceCallback::FActionCancelGoalServiceCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  Controller = Cast<URController>(InController);
}

void FActionCancelGoalServiceCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (Controller)
  {
    Controller->bCancel = true;
  }
  else
  {
    UE_LOG(LogRActionCancelGoalService, Error, TEXT("Controller not found"));
  }
}

TSharedPtr<FROSBridgeMsg> FActionCancelGoalServiceCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<actionlib_msgs::GoalID> GoalId =
      MakeShareable<actionlib_msgs::GoalID>(new actionlib_msgs::GoalID());

  GoalId->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(GoalId);
}