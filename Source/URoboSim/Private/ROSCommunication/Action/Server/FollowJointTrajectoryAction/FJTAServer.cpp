#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAServer.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTACancelSubscriber.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAFeedbackPublisher.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAGoalSubscriber.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAResultPublisher.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAStatusPublisher.h"

URFJTAServer::URFJTAServer()
{
  ActionName = TEXT("/whole_body_controller/body/follow_joint_trajectory");
  FrameId = TEXT("odom");
  JointParamTopic = TEXT("whole_body_controller/body/joints");
  ControllerName = TEXT("JointTrajectoryController");
}

void URFJTAServer::SetActionServerParameters(URActionServerParameter *&ActionServerParameters)
{
  URFJTAServerParameter *FJTAServerParameter = Cast<URFJTAServerParameter>(ActionServerParameters);
  if (FJTAServerParameter)
  {
    Super::SetActionServerParameters(ActionServerParameters);
    FrameId = FJTAServerParameter->FrameId;
    JointParamTopic = FJTAServerParameter->JointParamTopic;
  }
}

void URFJTAServer::CreateActionServer()
{
  CancelSubscriber = NewObject<URFJTACancelGoalService>(GetOwner());
  StatusPublisher = NewObject<URFJTAStatusPublisher>(GetOwner());
  ResultPublisher = NewObject<URFJTAGetResultService>(GetOwner());
  GoalSubscriber = NewObject<URFJTASendGoalService>(GetOwner());
  FeedbackPublisher = NewObject<URFJTAFeedbackPublisher>(GetOwner());

  Cast<URFJTAFeedbackPublisher>(FeedbackPublisher)->FrameId = FrameId;
  Cast<URFJTAGetResultService>(ResultPublisher)->FrameId = FrameId;
  Cast<URFJTAStatusPublisher>(StatusPublisher)->FrameId = FrameId;

  Cast<URFJTAFeedbackPublisher>(FeedbackPublisher)->JointParamTopic = JointParamTopic;
}

