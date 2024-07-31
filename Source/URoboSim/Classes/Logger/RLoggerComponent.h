#pragma once

#include "RPluginComponent.h"
#include "Logger/RLogger.h"
#include "SeqLog.h"
// clang-format off
#include "RLoggerComponent.generated.h"
// clang-format on

UENUM()
enum class UDataSink : uint8
{
  Seq
};


UCLASS()
class UROBOSIM_API URLoggerComponent : public URPluginComponent
{
	GENERATED_BODY()

protected:
	void Init() override;
	void Log(const FString&);

public:
	URLoggerComponent();
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void AddLogger(URLogger*& Logger) { Loggers.Add(Logger); }
	URLogger* GetLogger(const FString& LoggerName) const;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<URLogger *> Loggers;


	UPROPERTY(EditAnywhere)
  UDataSink Sink;
};
