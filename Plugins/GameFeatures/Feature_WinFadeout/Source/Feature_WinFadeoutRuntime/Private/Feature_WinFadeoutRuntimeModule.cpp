// Copyright Epic Games, Inc. All Rights Reserved.

#include "Feature_WinFadeoutRuntimeModule.h"

#define LOCTEXT_NAMESPACE "FFeature_WinFadeoutRuntimeModule"

void FFeature_WinFadeoutRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// the exact timing is specified in the .uplugin file per-module
}

void FFeature_WinFadeoutRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFeature_WinFadeoutRuntimeModule, Feature_WinFadeoutRuntime)
