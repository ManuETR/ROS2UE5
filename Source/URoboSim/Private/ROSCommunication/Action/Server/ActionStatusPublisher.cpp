#include "ROSCommunication/Action/Server/ActionStatusPublisher.h"
#include "action_msgs/msg/GoalStatusArray.h"

DEFINE_LOG_CATEGORY_STATIC(LogRActionStatusPublisher, Log, All)

URActionStatusPublisher::URActionStatusPublisher()
{
  MessageType = TEXT("action_msgs/msg/GoalStatusArray");
  FrameId = TEXT("odom");
}

void URActionStatusPublisher::Publish()
{
  if (Controller)
  {
    TSharedPtr<actionlib_msgs::GoalStatusArray> GoalStatusArrayMsg =
        MakeShareable(new actionlib_msgs::GoalStatusArray());

    GoalStatusArrayMsg->SetHeader(std_msgs::Header(FROSTime(), FrameId));

    TArray<actionlib_msgs::GoalStatus> GoalStatusArray;
    for (const FGoalStatusInfo &GoalStatusInfo : Controller->GetGoalStatusList())
    {
      actionlib_msgs::GoalStatus GoalStatus(actionlib_msgs::GoalID(FROSTime(GoalStatusInfo.Secs, GoalStatusInfo.NSecs), GoalStatusInfo.Id), GoalStatusInfo.Status, TEXT(""));
      GoalStatusArray.Add(GoalStatus);
    }
    GoalStatusArrayMsg->SetStatusList(GoalStatusArray);

    Handler->PublishMsg(Topic, GoalStatusArrayMsg);
    Handler->Process();
  }
  else
  {
    UE_LOG(LogRActionStatusPublisher, Error, TEXT("Controller not found in %s"), *GetName())
  }
}