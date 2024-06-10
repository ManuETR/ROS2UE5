#include "ROSCommunication/Publisher/JointStatePublisher.h"
#include "Physics/RJoint.h"
#include "sensor_msgs/JointState.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointStatePublisher, Log, All);

URJointStatePublisher::URJointStatePublisher()
{
  Topic = TEXT("/joint_states");
  MessageType = TEXT("sensor_msgs/JointState");
  JointParamTopic = TEXT("/hardware_interface/joints");
  FrameId = TEXT("odom");
}

void URJointStatePublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  if (URJointStatePublisherParameter *JointStatePublisherParameter = Cast<URJointStatePublisherParameter>(PublisherParameters))
  {
    Super::SetPublishParameters(PublisherParameters);
    JointParamTopic = JointStatePublisherParameter->JointParamTopic;
  }
}

void URJointStatePublisher::Init()
{
  Super::Init();
  if (GetOwner())
  {
    ConfigClient = NewObject<URJointStateConfigurationClient>(GetOwner());
    ConfigClient->JointParamTopic = JointParamTopic;
    ConfigClient->Connect(Handler);
    ConfigClient->GetJointNames([this](const TArray<FString> &JointNames){ ListJointName = JointNames; });
  }
}

void URJointStatePublisher::Publish()
{
  if (GetOwner())
  {
    TArray<URJoint*> Joints = GetOwner()->GetJoints();

    ListJointPosition.Empty();
    ListJointPosition.Reserve(Joints.Num());
    ListJointVelocity.Empty();
    ListJointVelocity.Reserve(Joints.Num());
    ListJointEffort.Empty();
    ListJointEffort.Reserve(Joints.Num());
    ListJointName.Empty();
    ListJointName.Reserve(Joints.Num());


    for (URJoint *Joint : Joints)
    {

      float JointPosition = Joint->GetEncoderValue();
      float JointVelocity = Joint->GetJointVelocity();

      ListJointPosition.Add(JointPosition);
      ListJointVelocity.Add(JointVelocity);
      ListJointEffort.Add(0.0);
      ListJointName.Add(Joint->GetName());

    }

    TSharedPtr<sensor_msgs::JointState> JointState =
        MakeShareable(new sensor_msgs::JointState());
    JointState->SetHeader(std_msgs::Header(FROSTime(), FrameId));
    JointState->SetName(ListJointName);
    JointState->SetPosition(ListJointPosition);
    JointState->SetVelocity(ListJointVelocity);
    JointState->SetEffort(ListJointEffort);

    Handler->PublishMsg(Topic, JointState);

    Handler->Process();

    // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
  }
}
