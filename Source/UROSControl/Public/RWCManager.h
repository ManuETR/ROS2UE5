// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "UROSCallbackRegisterBase.h"
#include "ROSBridgeHandler.h"
#include "RWCManager.generated.h"


UCLASS(Blueprintable, DefaultToInstanced, hidecategories = Object, editinlinenew)
class UROSCONTROL_API URWCManager : public UROSCallbackRegisterBase
{
	GENERATED_BODY()

public:
	URWCManager(){}
	~URWCManager(){}

	virtual void SetupServiceServers();

	virtual void Register(FString DefaultNamespace, UWorld* InWorld = nullptr) override;
	virtual void ConnectToHandler(const TSharedPtr<FROSBridgeHandler>& ROSHandler);

private:
	FString Namespace;
        UWorld* World;
};
