// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_WithInterface.h"


#include "EngineUtils.h"
#include "GameFeaturesSubsystemSettings.h"
#include "AssetRegistry/AssetBundleData.h"
#include "Engine/GameInstance.h"
#include "Engine/AssetManager.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#define LOCTEXT_NAMESPACE "GameFeatures"

void UGameFeatureAction_WithInterface::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	FContextHandles& Handles = ContextHandles.FindOrAdd(Context);
	
	Handles.GameInstanceStartHandle = FWorldDelegates::OnStartGameInstance.AddUObject(
		this,
		&UGameFeatureAction_WithInterface::HandleGameInstanceStart,
		FGameFeatureStateChangeContext(Context)
	);
	
	if (ensure(Handles.InterfaceRequestHandles.Num() == 0)) { 
		Handles.InterfaceRequestHandles.Empty();
	}
	
	// Add to any worlds with associated game instances that have already been initialized
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())	
	{
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			AddToWorld(WorldContext, FGameFeatureStateChangeContext(Context), Handles);
		}
	}
}

void UGameFeatureAction_WithInterface::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FContextHandles& Handles = ContextHandles.FindOrAdd(Context);
	
	FWorldDelegates::OnStartGameInstance.Remove(Handles.GameInstanceStartHandle);
	
	// Releasing the handles will also remove the components from any registered actors too
	Handles.InterfaceRequestHandles.Empty();
}


#if WITH_EDITORONLY_DATA
void UGameFeatureAction_WithInterface::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	if (UAssetManager::IsInitialized())
	{
		for (const FGameFeatureWithInterfaceConfig& Entry : ComponentList)
		{
			if (Entry.bClientComponent)
			{
				AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, Entry.ComponentClass.ToSoftObjectPath().GetAssetPath());
			}
			if (Entry.bServerComponent)
			{
				AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, Entry.ComponentClass.ToSoftObjectPath().GetAssetPath());
			}
		}
	}
}
#endif


#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_WithInterface::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 EntryIndex = 0;
	for (const FGameFeatureWithInterfaceConfig& Entry : ComponentList)
	{
		if (Entry.InterfaceClass.IsNull())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("ComponentEntryHasNullActor", "Null InterfaceClass at index {0} in ComponentList"), FText::AsNumber(EntryIndex)));
		}

		if (Entry.ComponentClass.IsNull())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("ComponentEntryHasNullComponent", "Null ComponentClass at index {0} in ComponentList"), FText::AsNumber(EntryIndex)));
		}

		++EntryIndex;
	}

	return Result;
}
#endif


/**
 * Add feature to specified world.
 *
 * Will also go through already existing actors and attach components to them
 */
void UGameFeatureAction_WithInterface::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& GameFeatureContext, FContextHandles& WorldContextHandles)
{
	const UWorld* World = WorldContext.World();
	const UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	
	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{

		UGKInterfaceSubsystem* InterfaceSubsystem = UGameInstance::GetSubsystem<UGKInterfaceSubsystem>(GameInstance);

		for (auto ComponentConfig : ComponentList)
		{
						
			/*
			 * Add extension handler for any new actors to be registered in future
			 */
		
			auto WrappedHandleInterfaceChange = [this, &WorldContext, GameFeatureContext](UGKInterfaceComponent* InterfaceComponent, FName EventName)
			{
				// Now you can call the actual function with all necessary parameters
				this->HandleInterfaceChange(InterfaceComponent, EventName, WorldContext, GameFeatureContext);
			};

			const FExtensionHandlerDelegate InterfaceChangeDelegate = FExtensionHandlerDelegate::CreateLambda(WrappedHandleInterfaceChange);
			auto Handle =  InterfaceSubsystem->AddExtensionHandler(ComponentConfig.InterfaceClass, InterfaceChangeDelegate);
			WorldContextHandles.InterfaceRequestHandles.Add(Handle);
		}
	}
}

void UGameFeatureAction_WithInterface::HandleGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext GameFeatureContext)
{
	if (const FWorldContext* WorldContext = GameInstance->GetWorldContext())
	{
		if (GameFeatureContext.ShouldApplyToWorldContext(*WorldContext))
		{	
			FContextHandles* Handles = ContextHandles.Find(GameFeatureContext);
			if (ensure(Handles))
			{
				AddToWorld(*WorldContext, FGameFeatureStateChangeContext(GameFeatureContext), *Handles);
			}
		}
	}
}

void UGameFeatureAction_WithInterface::HandleInterfaceChange(
	UGKInterfaceComponent* InterfaceComponent,
	FName EventName,
	const FWorldContext& WorldContext,
	const FGameFeatureStateChangeContext& ChangeContext)
{
	FContextHandles& Handles = ContextHandles.FindOrAdd(ChangeContext);

	if (EventName == UGKInterfaceSubsystem::NAME_InterfaceAdded)
	{
		TryAddComponentsToInterfacedActor(InterfaceComponent, WorldContext);
	}

	if (EventName == UGKInterfaceSubsystem::NAME_InterfaceRemoved)
	{
		TryRemoveComponentsFromInterfacedActor(InterfaceComponent, WorldContext);
	}
}

#undef LOCTEXT_NAMESPACE

void UGameFeatureAction_WithInterface::TryAddComponentsToInterfacedActor(UGKInterfaceComponent* InterfaceComponent, const FWorldContext& WorldContext)
{
	if (!ensure(InterfaceComponent))
	{
		return;
	}
	
	const UWorld* World = WorldContext.World();
	
	AActor* Actor = InterfaceComponent->GetOwner(); 
	
	const ENetMode NetMode = World->GetNetMode();
	const bool bIsServer = NetMode != NM_Client;
	const bool bIsClient = NetMode != NM_DedicatedServer;

	auto& ComponentInstances = InterfaceToComponentInstanceMap.FindOrAdd(InterfaceComponent);
	
	for (const FGameFeatureWithInterfaceConfig& Entry : ComponentList)
	{

		if (Entry.InterfaceClass != InterfaceComponent->GetClass())
		{
			continue;
		}
		
		const bool bShouldAddRequest = (bIsServer && Entry.bServerComponent) || (bIsClient && Entry.bClientComponent);
		if (!bShouldAddRequest)
		{
			continue;
		}
		
		TSubclassOf<UActorComponent> ComponentClass = Entry.ComponentClass.LoadSynchronous();
		if (ComponentClass)
		{
			UE_SCOPED_ENGINE_ACTIVITY(TEXT("Adding component to actor %s (%s)"), *Actor->GetHumanReadableName(), *Entry.ComponentClass.ToString());
			
			auto ComponentInstance = UGKInterfaceSubsystem::CreateComponentOnActor(Actor, ComponentClass);
			if (ComponentInstance)
			{
				ComponentInstances.Add(ComponentInstance);
			}
		}
		else if (!Entry.ComponentClass.IsNull())
		{
			// If class is set, but we couldn't load it
			UE_LOG(LogGameFeatures, Error, TEXT("[GameFeatureData %s]: Failed to load component class %s. Not applying component."), *GetPathNameSafe(this), *Entry.ComponentClass.ToString());
		}
				
	}
}

void UGameFeatureAction_WithInterface::TryRemoveComponentsFromInterfacedActor(UGKInterfaceComponent* InterfaceComponent,
	const FWorldContext& WorldContext)
{
	if (!ensure(InterfaceComponent))
	{
		return;
	}
	
	auto ComponentInstances = InterfaceToComponentInstanceMap.FindOrAdd(InterfaceComponent);

	for (auto ComponentInstance : ComponentInstances)
	{
		if (!ComponentInstance.IsValid())
		{
			continue;
		}
		
		UGKInterfaceSubsystem::RemoveComponentFromActor(ComponentInstance.Get());
	}

}
