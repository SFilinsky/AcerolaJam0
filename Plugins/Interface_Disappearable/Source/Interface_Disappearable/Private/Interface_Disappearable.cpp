// Copyright Epic Games, Inc. All Rights Reserved.

#include "Interface_Disappearable.h"

#define LOCTEXT_NAMESPACE "FInterface_DisappearableModule"

void FInterface_DisappearableModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FInterface_DisappearableModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInterface_DisappearableModule, Interface_Disappearable)