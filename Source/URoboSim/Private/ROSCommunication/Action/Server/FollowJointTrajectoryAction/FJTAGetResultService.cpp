#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAResultPublisher.h"
#include "control_msgs/action/FollowJointTrajectory_GetResult.h"
#include "control_msgs/FollowJointTrajectoryResult.h"

DEFINE_LOG_CATEGORY_STATIC(LogRFJTAResultPublisher, Log, All);

URFJTAGetResultService::URFJTAGetResultService()
{
  MessageType = TEXT("control_msgs/action/FollowJointTrajectory_GetResult");
  FrameId = TEXT("odom");
}

void URFJTAGetResultService::Init()
{
  Super::Init();
  JointTrajectoryController = Cast<URJointTrajectoryController>(Controller);
  if (JointTrajectoryController)
  {
    JointTrajectoryController->ActionFinished.AddDynamic(this, &URFJTAGetResultService::PublishResult);
    if( JointTrajectoryController->ActionFinished.IsBound() )
      {
        UE_LOG(LogTemp, Error, TEXT("Result is bound"));
      }
  }
}

void URFJTAGetResultService::CreateServiceServer()
{
  if (Controller)
  {
    ServiceServer = MakeShareable<URFJTASendGoalServiceCallback>(
        new URFJTASendGoalServiceCallback(Topic, MessageType, Controller));
  }
}

void URFJTAGetResultService::PublishResult(FGoalStatusInfo InStatusInfo)
{
  // if(Owner->bPublishResult)
  UE_LOG(LogTemp, Error, TEXT("Enter publish result"));
  if (GetOwner() && JointTrajectoryController)
  {

    UE_LOG(LogTemp, Error, TEXT("Publish Result FollowJointTrajectory"));
    TSharedPtr<control_msgs::action::FollowJointTrajectory_GetResult> ActionResult =
        MakeShareable(new control_msgs::action::FollowJointTrajectory_GetResult());

    ActionResult->SetHeader(std_msgs::Header(FROSTime(), FrameId));

    // uint8 status = Owner->Status;
    actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(InStatusInfo.Secs, InStatusInfo.NSecs), InStatusInfo.Id), InStatusInfo.Status, "");
    ActionResult->SetStatus(GS);

    control_msgs::FollowJointTrajectoryResult Result(0);
    ActionResult->SetResult(Result);

    Handler->PublishMsg(Topic, ActionResult);
    Handler->Process();


    // Owner->GoalStatusList.RemoveSingle(Status);
    UE_LOG(LogTemp, Error, TEXT("Publish finished"));

    // while(IndexArray.Num() != 0)
    //   {
    //     Index = IndexArray.Pop();
    //     Owner->GoalStatusList.RemoveAt(Index);
    //   }
    // for(auto& i : IndexArray)
    //   {
    //     Owner->GoalStatusList.RemoveAt(i);
    //   }
  }
}
