// Copyright Epic Games, Inc. All Rights Reserved.

#include "Feature_BeatManagerRuntimeModule.h"

#define LOCTEXT_NAMESPACE "FFeature_BeatManagerRuntimeModule"

void FFeature_BeatManagerRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// the exact timing is specified in the .uplugin file per-module
}

void FFeature_BeatManagerRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFeature_BeatManagerRuntimeModule, Feature_BeatManagerRuntime)
