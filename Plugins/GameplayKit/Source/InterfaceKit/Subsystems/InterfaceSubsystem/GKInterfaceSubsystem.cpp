// Fill out your copyright notice in the Description page of Project Settings.


#include "GKInterfaceSubsystem.h"

#include "InterfaceKit.h"

/*
 * FInterfaceExtensionHandle
 */

FInterfaceExtensionHandle::~FInterfaceExtensionHandle()
{
	UGKInterfaceSubsystem* LocalManager = OwningManager.Get();
	if (LocalManager)
	{
		if (ExtensionHandle.IsValid())
		{
			LocalManager->RemoveExtensionHandler(InterfaceClass, ExtensionHandle);
		}
	}
}

bool FInterfaceExtensionHandle::IsValid() const
{
	return OwningManager.IsValid();
}




/*
 * UGKInterfaceSubsystem
 */

UGKInterfaceSubsystem::UGKInterfaceSubsystem(const FObjectInitializer& ObjectInitializer)
{
}

UGKInterfaceSubsystem* UGKInterfaceSubsystem::GetForActor(const AActor* Actor, bool bOnlyGameWorlds)
{
	if (Actor)
	{
		if (UWorld* ReceiverWorld = Actor->GetWorld())
		{
			if (bOnlyGameWorlds && (!ReceiverWorld->IsGameWorld() || ReceiverWorld->IsPreviewWorld()))
			{
				return nullptr;
			}

			return UGameInstance::GetSubsystem<UGKInterfaceSubsystem>(ReceiverWorld->GetGameInstance());
		}
	}

	return nullptr;
}

void UGKInterfaceSubsystem::AddInterfaceReceiver(AActor* Receiver, bool bAddOnlyInGameWorlds)
{
	if (UGKInterfaceSubsystem* InterfaceSubsystem = GetForActor(Receiver, bAddOnlyInGameWorlds))
	{
		InterfaceSubsystem->AddReceiverInternal(Receiver);
	}
}

void UGKInterfaceSubsystem::RemoveInterfaceReceiver(AActor* Receiver)
{
	if (UGKInterfaceSubsystem* InterfaceSubsystem = GetForActor(Receiver))
	{
		InterfaceSubsystem->RemoveReceiverInternal(Receiver);
	}
}

void UGKInterfaceSubsystem::AddReceiver(AActor* Receiver, bool bAddOnlyInGameWorlds)
{
	if (Receiver != nullptr)
	{
		if (bAddOnlyInGameWorlds)
		{
			UWorld* ReceiverWorld = Receiver->GetWorld();
			if ((ReceiverWorld == nullptr) || !ReceiverWorld->IsGameWorld() || ReceiverWorld->IsPreviewWorld())
			{
				return;
			}
		}

		AddReceiverInternal(Receiver);
	}
}

void UGKInterfaceSubsystem::RemoveReceiver(AActor* Receiver)
{
	if (Receiver != nullptr)
	{
		RemoveReceiverInternal(Receiver);
	}
}

TSharedPtr<FInterfaceExtensionHandle> UGKInterfaceSubsystem::AddExtensionHandler(
	const TSoftClassPtr<UGKInterfaceComponent>& InterfaceClass,
	FExtensionHandlerDelegate ExtensionHandler)
{
	// You must have a target and bound handler
	if (!ensure(!InterfaceClass.IsNull()) || !ensure(ExtensionHandler.IsBound()))
	{
		return nullptr;
	}

	FInterfaceClassPath InterfaceClassPath(InterfaceClass);
	FExtensionHandlerEventMap& HandlerEventMap = InterfaceToExtensionHandlerMap.FindOrAdd(InterfaceClassPath);

	// This is a fake multicast delegate using a map
	FDelegateHandle DelegateHandle(FDelegateHandle::EGenerateNewHandleType::GenerateNewHandle);
	HandlerEventMap.Add(DelegateHandle, ExtensionHandler);

	if (UClass* InterfaceClassPtr = InterfaceClass.Get())
	{
		CleanInvalidReceivers();
		for (TWeakObjectPtr<AActor> Actor : AllReceivers)
		{
			if (Actor->IsActorInitialized())
			{
				auto InterfaceComponent = GetInterfaceIfImplements(Actor.Get(), InterfaceClass);
				if (InterfaceComponent != nullptr)
				{
					ExtensionHandler.Execute(Cast<UGKInterfaceComponent>(InterfaceComponent), NAME_InterfaceAdded);
				}
			}
		}
	}
	else
	{
		// Actor class is not in memory, there will be no actor instances
	}

	return MakeShared<FInterfaceExtensionHandle>(this, InterfaceClass, DelegateHandle);
}

void UGKInterfaceSubsystem::NotifyInterfaceAddition(AActor* Actor, TSubclassOf<UGKInterfaceComponent> InterfaceClass)	
{
	if (!AllReceivers.Contains(Actor))
	{
		#if WITH_EDITOR
			UE_LOG(LogInterfaceKit, Warning, TEXT("%s: Interface %s was added to %s , but it's not registered as receiver."), *this->GetName(), *InterfaceClass->GetName(), *AActor::GetDebugName(Actor));
		#endif
		
		return;
	}

	
	UE_LOG(LogInterfaceKit, Display, TEXT("%s: Interface %s was added to registered receiver actor %s."), *this->GetName(), *InterfaceClass->GetName(), *AActor::GetDebugName(Actor));

	TriggerHandlersForInterface(Actor, NAME_InterfaceAdded, InterfaceClass);
}

void UGKInterfaceSubsystem::NotifyInterfaceRemoval(AActor* Actor, TSubclassOf<UGKInterfaceComponent> InterfaceClass)
{
	if (!AllReceivers.Contains(Actor))
	{
		#if WITH_EDITOR
				UE_LOG(LogInterfaceKit, Display, TEXT("%s: Interface %s was removed from %s, but it's not registered as receiver."), *this->GetName(), *InterfaceClass->GetName(), *AActor::GetDebugName(Actor));
		#endif
			
		return;
	}

	
	UE_LOG(LogInterfaceKit, Display, TEXT("%s: Interface %s was removed from registered receiver actor %s."), *this->GetName(), *InterfaceClass->GetName(), *AActor::GetDebugName(Actor));

	TriggerHandlersForInterface(Actor, NAME_InterfaceRemoved, InterfaceClass);
}


UActorComponent* UGKInterfaceSubsystem::CreateComponentOnActor(AActor* ActorInstance, TSubclassOf<UActorComponent> ComponentClass)
{
	check(ActorInstance);
	check(ComponentClass);
	
	if (!ComponentClass->GetDefaultObject<UActorComponent>()->GetIsReplicated() || ActorInstance->GetLocalRole() == ROLE_Authority)
	{
		UActorComponent* NewComp = NewObject<UActorComponent>(ActorInstance, ComponentClass, ComponentClass->GetFName());
		
		if (USceneComponent* NewSceneComp = Cast<USceneComponent>(NewComp))
		{
			NewSceneComp->SetupAttachment(ActorInstance->GetRootComponent());
		}

		NewComp->RegisterComponent();

		return NewComp;
	}
	
	return nullptr;
}

void UGKInterfaceSubsystem::RemoveComponentFromActor(UActorComponent* ComponentInstance)
{
	if (IsValid(ComponentInstance))
	{
		// Detach the component from its parent actor
		if (USceneComponent* SceneComponentInstance = Cast<USceneComponent>(ComponentInstance))
		{
			SceneComponentInstance->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		}
		
		// Mark the component for destruction
		ComponentInstance->DestroyComponent();
	}
}

TArray<UGKInterfaceComponent*> UGKInterfaceSubsystem::GetInterfaceClassInstances(
	const TSoftClassPtr<UGKInterfaceComponent>& InterfaceClass)
{
	TArray<UGKInterfaceComponent*> InterfaceInstances;
	for (const auto Receiver : AllReceivers)
	{
		const auto ReceiverInst = Receiver.Get();
		if (ReceiverInst == nullptr)
		{
			continue;
		}

		const auto InterfaceInst = GetInterfaceIfImplements(ReceiverInst, InterfaceClass);
		if (InterfaceInst == nullptr)
		{
			continue;
		}
		
		InterfaceInstances.Add(InterfaceInst);
	}

	return InterfaceInstances;
}

void UGKInterfaceSubsystem::AddReceiverInternal(AActor* Receiver)
{
	checkSlow(Receiver);

	CleanInvalidReceivers();
	AllReceivers.Add(TWeakObjectPtr<AActor>(Receiver));
	
	TriggerAllHandlersForReceiver(Receiver, NAME_InterfaceAdded);
}

void UGKInterfaceSubsystem::RemoveReceiverInternal(AActor* Receiver)
{
	checkSlow(Receiver);
	
	AllReceivers.Remove(Receiver);
	CleanInvalidReceivers();

	TriggerAllHandlersForReceiver(Receiver, NAME_InterfaceRemoved);
}


void UGKInterfaceSubsystem::RemoveExtensionHandler(const TSoftClassPtr<UGKInterfaceComponent>& InterfaceClass,
                                                   FDelegateHandle DelegateHandle)
{
	const auto InterfaceClassPath = FInterfaceClassPath(InterfaceClass);
	
	const auto ExtensionHandlers = InterfaceToExtensionHandlerMap.Find(InterfaceClassPath);
	if (ExtensionHandlers != nullptr)
	{
		ExtensionHandlers->Remove(DelegateHandle);
	}
}

void UGKInterfaceSubsystem::TriggerAllHandlersForReceiver(const AActor* Actor, FName Event)
{

	if (!ensure(Actor))
	{
		return;
	}
	
	TArray<UGKInterfaceComponent*> Components;
	Actor->GetComponents(UGKInterfaceComponent::StaticClass(), Components, false);
	
	for (const auto& InterfaceComponent : Components)
	{
		const auto InterfaceClass = InterfaceComponent->GetClass();
		TriggerHandlersForInterface(Actor, Event, InterfaceClass);
	}
}

void UGKInterfaceSubsystem::TriggerHandlersForInterface(const AActor* Receiver, FName Event,
	TSubclassOf<UGKInterfaceComponent> InterfaceClass)
{
	if (!ensure(InterfaceClass))
	{
		return;
	}

	TSoftClassPtr<UGKInterfaceComponent> InterfaceClassPtr = TSoftClassPtr<UGKInterfaceComponent>(InterfaceClass);

	UGKInterfaceComponent* InterfaceComponent = GetInterfaceIfImplements(Receiver, InterfaceClassPtr);
	if (InterfaceComponent == nullptr)
	{
		return;
	}

	for (const auto& Elem : InterfaceToExtensionHandlerMap)
	{

		const auto MapInterface = Elem.Key.InterfaceClass;
		if (!MapInterface.IsValid())
		{
			continue;
		}

		if (!InterfaceComponent->IsA(MapInterface.Get()))
		{
			continue;
		}
		
		const auto HandlerMapPtr = Elem.Value;
	
		for (const auto& HandlerMapElem : HandlerMapPtr)
		{
			const auto Handler = HandlerMapElem.Value;
	
			Handler.Execute(InterfaceComponent, Event);
		}
	}
	
	
}

void UGKInterfaceSubsystem::CleanInvalidReceivers()
{
	for (auto It = AllReceivers.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}
}


UGKInterfaceComponent* UGKInterfaceSubsystem::GetInterfaceIfImplements(const AActor* Actor,
																 const TSoftClassPtr<UGKInterfaceComponent>& InterfaceClassPtr)
{
	if (Actor == nullptr)
	{
		return nullptr;
	}
	
	if (!InterfaceClassPtr.IsValid())
	{
		return nullptr;
	}

	const auto ComponentClass = InterfaceClassPtr.LoadSynchronous();

	const auto ComponentInst = Actor->GetComponentByClass(ComponentClass);
	
	return Cast<UGKInterfaceComponent>(ComponentInst);
}


