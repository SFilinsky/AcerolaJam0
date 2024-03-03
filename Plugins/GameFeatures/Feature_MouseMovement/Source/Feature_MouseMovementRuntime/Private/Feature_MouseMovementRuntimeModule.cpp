// Copyright Epic Games, Inc. All Rights Reserved.

#include "Feature_MouseMovementRuntimeModule.h"

#define LOCTEXT_NAMESPACE "FFeature_MouseMovementRuntimeModule"

DEFINE_LOG_CATEGORY(LogMouseMovement)

void FFeature_MouseMovementRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// the exact timing is specified in the .uplugin file per-module
}

void FFeature_MouseMovementRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFeature_MouseMovementRuntimeModule, Feature_MouseMovementRuntime)
