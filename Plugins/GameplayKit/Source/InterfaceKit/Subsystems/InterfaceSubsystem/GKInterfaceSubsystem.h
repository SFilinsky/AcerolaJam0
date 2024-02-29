// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "InterfaceRequest/GKInterfaceRequest.h"
#include "GKInterfaceSubsystem.generated.h"

class UGKInterfaceSubsystem;

struct FInterfaceExtensionHandle
{
	FInterfaceExtensionHandle(const TWeakObjectPtr<UGKInterfaceSubsystem>& InOwningManager, const TSoftClassPtr<UGKInterfaceComponent>& InReceiverClass, const TSubclassOf<UActorComponent>& InComponentClass)
		: OwningManager(InOwningManager)
		, InterfaceClass(InReceiverClass)
		, ComponentClass(InComponentClass)
	{}

	FInterfaceExtensionHandle(const TWeakObjectPtr<UGKInterfaceSubsystem>& InOwningManager, const TSoftClassPtr<UGKInterfaceComponent>& InReceiverClass, FDelegateHandle InExtensionHandle)
		: OwningManager(InOwningManager)
		, InterfaceClass(InReceiverClass)
		, ExtensionHandle(InExtensionHandle)
	{}

	INTERFACEKIT_API ~FInterfaceExtensionHandle();

	/** Returns true if the manager that this request is for still exists */
	INTERFACEKIT_API bool IsValid() const;

private:
	/** The manager that this request was for */
	TWeakObjectPtr<UGKInterfaceSubsystem> OwningManager;

	/** The class of interface to put components */
	TSoftClassPtr<UGKInterfaceComponent> InterfaceClass;

	/** The class of component to put on actors with interface */
	TSubclassOf<UActorComponent> ComponentClass;

	/** A handle to an extension delegate to run */
	FDelegateHandle ExtensionHandle;
};


UCLASS()
class INTERFACEKIT_API UGKInterfaceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	friend struct FInterfaceExtensionHandle;

	/** Using a fake multicast delegate so order can be kept consistent */
	DECLARE_DELEGATE_TwoParams(FExtensionHandlerDelegateInternal, UGKInterfaceComponent*, FName);
	using FExtensionHandlerEventMap = TMap<FDelegateHandle, FExtensionHandlerDelegateInternal>;
public:
	/** Delegate types for extension handlers */
	using FExtensionHandlerDelegate = FExtensionHandlerDelegateInternal;
	
public:

	UGKInterfaceSubsystem(const FObjectInitializer& ObjectInitializer);
	
	/* Static API */
	
	/** Utility to get this manager from an actor, will return null if actor is null or not in a world */
	static UGKInterfaceSubsystem* GetForActor(const AActor* Actor, bool bOnlyGameWorlds = true);

	/** Adds an actor as a receiver for components (automatically finding the manager for the actor's  game instance). If it passes the actorclass filter on requests it will get the components. */
	static void AddInterfaceReceiver(AActor* Receiver, bool bAddOnlyInGameWorlds = true);

	/** Removes an actor as a receiver for components (automatically finding the manager for the actor's game instance). */
	static void RemoveInterfaceReceiver(AActor* Receiver);
	
	/* Static Ends */

	/** Adds an actor as a receiver for components. If it passes the actorclass filter on requests it will get the components. */
	UFUNCTION(BlueprintCallable, Category="Gameplay", meta=(DefaultToSelf="Receiver", AdvancedDisplay=1))
	void AddReceiver(AActor* Receiver, bool bAddOnlyInGameWorlds = true);

	/** Removes an actor as a receiver for components. */
	UFUNCTION(BlueprintCallable, Category="Gameplay", meta=(DefaultToSelf="Receiver"))
	void RemoveReceiver(AActor* Receiver);
	
	/** Adds an extension handler to run on actors of the given class. Returns a handle that will keep the handler "alive" until it is destructed, at which point the delegate is removed */
	TSharedPtr<FInterfaceExtensionHandle> AddExtensionHandler(const TSoftClassPtr<UGKInterfaceComponent>& InterfaceClass, FExtensionHandlerDelegate ExtensionHandler);

	void NotifyInterfaceAddition(AActor* Actor, TSubclassOf<UGKInterfaceComponent> InterfaceClass);
	void NotifyInterfaceRemoval(AActor* Actor, TSubclassOf<UGKInterfaceComponent> InterfaceClass);
	
	/**
     * Creates component on actor instance and caches it in ComponentClassToComponentInstanceMap
     * @param ActorInstance 
     * @param ComponentClass 
     */
	static UActorComponent* CreateComponentOnActor(AActor* ActorInstance, TSubclassOf<UActorComponent> ComponentClass);
	static void RemoveComponentFromActor(UActorComponent* ComponentInstance);	
	
protected:
	
	void AddReceiverInternal(AActor* Receiver);
	void RemoveReceiverInternal(AActor* Receiver);
	

	/**
	 * Excludes extension handler from Map of delegates for specific Interface
	 * @param InterfaceClass 
	 * @param DelegateHandle 
	 */
	void RemoveExtensionHandler(const TSoftClassPtr<UGKInterfaceComponent>& InterfaceClass, FDelegateHandle DelegateHandle);

	/**
	 * Going to find all already registered extension handles that should be executed for the actor, and execute them
	 * @param Actor 
	 * @param Event 
	 */
	void TriggerAllHandlersForReceiver(const AActor* Actor, FName Event);
	
	void TriggerHandlersForInterface(const AActor* Receiver, FName Event, TSubclassOf<UGKInterfaceComponent> InterfaceClass);

	void CleanInvalidReceivers();
	
	UGKInterfaceComponent* GetInterfaceIfImplements(const AActor* Actor, const TSoftClassPtr<UGKInterfaceComponent>& InterfaceClassPtr);
	
/* State */

public:

	//////////////////////////////////////////////////////////////////////////////////////////////
	// The extension system allows registering for arbitrary event callbacks on receiver actors.
	// These are the default events but games can define, send, and listen for their own.


	inline static FName NAME_InterfaceAdded = FName("InterfaceAdded");
	inline static FName NAME_InterfaceRemoved = FName("InterfaceRemoved");

protected:

	TSet<TWeakObjectPtr<AActor>> AllReceivers;
	
	/** A map of actor classes to delegate handlers that should be executed for actors of that class. */
	TMap<FInterfaceClassPath, FExtensionHandlerEventMap> InterfaceToExtensionHandlerMap;

};

