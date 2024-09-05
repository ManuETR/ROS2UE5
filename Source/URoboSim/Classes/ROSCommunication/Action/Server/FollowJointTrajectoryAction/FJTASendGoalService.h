#pragma once

#include "Controller/ControllerType/JointController/RJointTrajectoryController.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "FJTASendGoalService.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTASendGoalService final: public URActionService
{
  GENERATED_BODY()

public:
  URFJTASendGoalService();

public:
  void CreateServiceServer() override;
};

class UROBOSIM_API URFJTASendGoalServiceCallback final : public FROSBridgeSrvServer
{
public:
	URFJTASendGoalServiceCallback(const FString& InTopic, const FString& InType, UObject* InController);

	TSharedPtr<control_msgs::action::FollowJointTrajectory_GetResult> ActionResult;
	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URJointTrajectoryController* JointTrajectoryController;
};