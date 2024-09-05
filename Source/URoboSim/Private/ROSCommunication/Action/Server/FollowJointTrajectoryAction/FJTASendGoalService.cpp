#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAGoalSubscriber.h"
#include "control_msgs/action/FollowJointTrajectory_SendGoal.h"

DEFINE_LOG_CATEGORY_STATIC(LogRFJTAGoalSubscriber, Log, All)

URFJTASendGoalService::URFJTASendGoalService()
{
  MessageType = TEXT("control_msgs/action/FollowJointTrajectory_SendGoal");
}

void URFJTASendGoalService::CreateServiceServer()
{
  if (Controller)
  {
    ServiceServer = MakeShareable<URFJTASendGoalServiceCallback>(
        new URFJTASendGoalServiceCallback(Topic, MessageType, Controller));
  }
}

URFJTASendGoalServiceCallback::URFJTASendGoalServiceCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  JointTrajectoryController = Cast<URJointTrajectoryController>(InController);
}

TSharedPtr<FROSBridgeMsg> URFJTASendGoalServiceCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<control_msgs::action::FollowJointTrajectory_SendGoal> JointStateMessage =
      MakeShareable<control_msgs::action::FollowJointTrajectory_SendGoal>(new control_msgs::action::FollowJointTrajectory_SendGoal());

  JointStateMessage->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMessage);
}

void URFJTASendGoalServiceCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (JointTrajectoryController)
  {
    TSharedPtr<control_msgs::action::FollowJointTrajectory_SendGoal> TrajectoryMsg = StaticCastSharedPtr<control_msgs::action::FollowJointTrajectory_SendGoal>(Msg);

    TArray<FString> JointNames = TrajectoryMsg->GetGoal().GetTrajectory().GetJointNames();
    actionlib_msgs::GoalID Id = TrajectoryMsg->GetGoalId();
    FGoalStatusInfo GoalStatusInfo = FGoalStatusInfo(Id.GetId(), Id.GetStamp().Secs, Id.GetStamp().NSecs);

    UE_LOG(LogTemp, Log, TEXT("%s Recieved Trajectory Id: %s"), *FROSTime::Now().ToString(), *Id.GetId());

    FROSTime ActionStart = TrajectoryMsg->GetGoal().GetTrajectory().GetHeader().GetStamp();

    TArray<FTrajectoryPoints> Trajectory;
    for (const trajectory_msgs::JointTrajectoryPoint &JointPoint : TrajectoryMsg->GetGoal().GetTrajectory().GetPoints())
    {
      FROSTime TimeStep(JointPoint.GetTimeFromStart());
      Trajectory.Add(FTrajectoryPoints(TimeStep.Secs, TimeStep.NSecs, JointNames, JointPoint.GetPositions(), JointPoint.GetVelocities()));
    }

    // JointTrajectoryController->FollowJointTrajectory(ActionStart.GetTimeAsDouble(), GoalStatusInfo, Trajectory);

    double ActionTimeDiff = ActionStart.GetTimeAsDouble() - FROSTime::Now().GetTimeAsDouble();
    if(ActionTimeDiff <= 0.0f)
      {
        UE_LOG(LogTemp, Error, TEXT("Start Trajectory Instantly"));
        JointTrajectoryController->FollowJointTrajectory(ActionStart.GetTimeAsDouble(), GoalStatusInfo, Trajectory);
      }
    else
      {
        UE_LOG(LogTemp, Error, TEXT("Start Trajectory Delayed %f"), ActionTimeDiff);
        FTimerHandle MyTimerHandle;
        FTimerDelegate StartTrajectoryDelegate = FTimerDelegate::CreateUObject( JointTrajectoryController,  &URJointTrajectoryController::FollowJointTrajectory, ActionStart.GetTimeAsDouble(), GoalStatusInfo, Trajectory);
        // FTimerDelegate StartTrajectoryDelegate = FTimerDelegate::BindUFunction( JointTrajectoryController,  FName("FollowJointTrajectory"), ActionStart.GetTimeAsDouble(), GoalStatusInfo, Trajectory);

        // FTimerDelegate StartTrajectoryDelegate = FTimerDelegate::CreateUObject( JointController,  &URJointController::FollowTrajectory, ActionStart.GetTimeAsDouble(), GoalStatusInfo, Names, Trajectory);
        // FTimerDelegate StartTrajectoryDelegate = FTimerDelegate::BindUFunction( this,  FName("FollowTrajectory"), ActionStart.GetTimeAsDouble(), GoalStatusInfo, Names, Trajectory);
        JointTrajectoryController->GetOwner()->GetWorldTimerManager().SetTimer(MyTimerHandle, StartTrajectoryDelegate, ActionTimeDiff, false);
      }
  }
  else
  {
    UE_LOG(LogRFJTAGoalSubscriber, Error, TEXT("JointController not found"))
  }
  UE_LOG(LogRFJTAGoalSubscriber, Error, TEXT("GoalReceivedFinished"))
}
