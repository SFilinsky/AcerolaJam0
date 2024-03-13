// Copyright Epic Games, Inc. All Rights Reserved.

#include "Feature_MoveActorRuntimeModule.h"

#define LOCTEXT_NAMESPACE "FFeature_MoveActorRuntimeModule"

void FFeature_MoveActorRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// the exact timing is specified in the .uplugin file per-module
}

void FFeature_MoveActorRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFeature_MoveActorRuntimeModule, Feature_MoveActorRuntime)
