// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#if ENGINE_MINOR_VERSION > 23 || ENGINE_MAJOR_VERSION >4
#include "Modules/ModuleManager.h"
#else
#include "ModuleManager.h"
#endif

class FUWorldControlModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
