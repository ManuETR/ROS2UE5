// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "UROSControl.h"

#define LOCTEXT_NAMESPACE "FUROSControl"

void FUROSControl::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FUROSControl::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUROSControl, UROSControl)
