#pragma once

#include "Controller/ControllerType/JointController/RJointTrajectoryController.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "FJTAGetResultService.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTAGetResultService final: public URActionService
{
  GENERATED_BODY()

public:
	URFJTAGetResultService();

	UPROPERTY(EditAnywhere)
	FString FrameId;

	void CreateServiceServer() override;

protected:
	void Init() override;
};

class UROBOSIM_API URFJTAGetResultServiceCallback final : public FROSBridgeSrvServer
{
public:
	URFJTAGetResultServiceCallback(const FString& InTopic, const FString& InType, UObject* InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URJointTrajectoryController* JointTrajectoryController;

	UFUNCTION()
	void PublishResult(FGoalStatusInfo InStatusInfo);
};