
#include "ROSCommunication/Subscriber/JointStateSubscriber.h"
#include "sensor_msgs/JointState.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointStateSubscriber, Log, All)

URJointStateSubscriber::URJointStateSubscriber()
{
  MessageType = TEXT("sensor_msgs/msg/JointState");
}

void URJointStateSubscriber::CreateSubscriber()
{
  if (GetOwner())
  {
    Subscriber = MakeShareable<FRJointStateSubscriberCallback>(
        new FRJointStateSubscriberCallback(Topic, MessageType, GetOwner()));
  }
}

FRJointStateSubscriberCallback::FRJointStateSubscriberCallback(
    const FString &InTopic, const FString &InType, ARModel *InModel) : FROSBridgeSubscriber(InTopic, InType)
{
  Model = InModel;
}

TSharedPtr<FROSBridgeMsg> FRJointStateSubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<sensor_msgs::JointState> JointStateMessage =
      MakeShareable<sensor_msgs::JointState>(new sensor_msgs::JointState());

  JointStateMessage->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMessage);
}

void FRJointStateSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  // Downcast to subclass using StaticCastSharedPtr function
  TSharedPtr<sensor_msgs::JointState> JointStateMessage = StaticCastSharedPtr<sensor_msgs::JointState>(Msg);

  for (int i = 0; i < JointStateMessage->Names.Num(); i++)
  {
    URJoint *Joint = Model->GetJoint(JointStateMessage->Names[i]);
    if (Joint)
    {
      if (JointStateMessage->Efforts.IsValidIndex(i)) {
        Joint->SetJointEffort(JointStateMessage->Efforts[i]);
      }
      if (JointStateMessage->Velocities.IsValidIndex(i)) {
        Joint->SetJointVelocity(JointStateMessage->Velocities[i]);
      }
      if (JointStateMessage->Positions.IsValidIndex(i)) {
        Joint->SetJointPosition(JointStateMessage->Positions[i], nullptr);
      }
    }

  }

  // Do something with the message
  // UE_LOG(LogTemp, Log, TEXT("[%s] Message received! Content: %s"), *FString(__FUNCTION__), *JointStateMessage->ToString());
}