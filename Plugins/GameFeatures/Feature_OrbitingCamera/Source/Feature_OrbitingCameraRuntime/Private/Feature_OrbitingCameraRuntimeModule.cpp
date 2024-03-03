// Copyright Epic Games, Inc. All Rights Reserved.

#include "Feature_OrbitingCameraRuntimeModule.h"

#define LOCTEXT_NAMESPACE "FFeature_OrbitingCameraRuntimeModule"

DEFINE_LOG_CATEGORY(LogCameraControls)

void FFeature_OrbitingCameraRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// the exact timing is specified in the .uplugin file per-module
}

void FFeature_OrbitingCameraRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFeature_OrbitingCameraRuntimeModule, Feature_OrbitingCameraRuntime)
