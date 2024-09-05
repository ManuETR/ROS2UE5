#pragma once

#include "RActionServer.h"
// clang-format off
#include "ActionCancelGoalService.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URActionCancelGoalService : public URActionSubscriber
{
	GENERATED_BODY()

public:
	URActionCancelGoalService();

public:
	virtual void CreateSubscriber() override;
};

class UROBOSIM_API FActionCancelGoalServiceCallback : public FROSBridgeSubscriber
{
public:
	FActionCancelGoalServiceCallback(const FString& InTopic, const FString& InType, UObject* InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	virtual void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URController* Controller;
};