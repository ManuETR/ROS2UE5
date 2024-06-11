// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#if ENGINE_MINOR_VERSION > 23 || ENGINE_MAJOR_VERSION >4
#include "Modules/ModuleManager.h"
#else
#include "ModuleManager.h"
#endif

class FUROSControl : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};
