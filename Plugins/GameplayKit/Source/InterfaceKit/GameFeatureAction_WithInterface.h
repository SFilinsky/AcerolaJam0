// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Subsystems/InterfaceSubsystem/GKInterfaceSubsystem.h"

#include "GameFeatureAction_WithInterface.generated.h"

class AGKActor;
class UActorComponent;
class UGameInstance;
struct FWorldContext;
class UGKInterfaceComponent;

USTRUCT()
struct INTERFACEKIT_API FGameFeatureWithInterfaceConfig {
	GENERATED_BODY()

	// The base actor class to add a component to
	UPROPERTY(EditAnywhere, Category="Components", meta=(AllowAbstract="True"))
	TSoftClassPtr<UGKInterfaceComponent> InterfaceClass;

	// The component class to add to the specified type of actor
	UPROPERTY(EditAnywhere, Category="Components")
	TSoftClassPtr<UActorComponent> ComponentClass;
	
	// Should this component be added for clients
	UPROPERTY(EditAnywhere, Category="Components")
	uint8 bClientComponent:1; 

	// Should this component be added on servers
	UPROPERTY(EditAnywhere, Category="Components")
	uint8 bServerComponent:1;

	FGameFeatureWithInterfaceConfig()
		: bClientComponent(true)
		, bServerComponent(true)
	{
	}
};


/**
 * Adds interface <-> component spawn request to the component manager 
 */
UCLASS(meta = (DisplayName = "Add Components To Interfaced Actors"))
class INTERFACEKIT_API UGameFeatureAction_WithInterface : public UGameFeatureAction
{
	GENERATED_BODY()
	
	using FExtensionHandlerDelegate = UGKInterfaceSubsystem::FExtensionHandlerDelegate; 
	
public:

	//~UGameFeatureAction interface
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	#if WITH_EDITORONLY_DATA
		virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
	#endif
	//~End of UGameFeatureAction interface


	//~UObject interface
	#if WITH_EDITOR
		virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	#endif
	//~End of UObject interface


	UPROPERTY(EditAnywhere, Category="Components", meta=(TitleProperty="{InterfaceClass} -> {ComponentClass}"))
	TArray<FGameFeatureWithInterfaceConfig> ComponentList;

private:
	struct FContextHandles {
		FDelegateHandle GameInstanceStartHandle;
		TArray<TSharedPtr<FInterfaceExtensionHandle>> InterfaceRequestHandles;
	};
	
	/** A map of component classes to instances of that component class made by this action */
	TMap<TWeakObjectPtr<UGKInterfaceComponent>, TSet<TWeakObjectPtr<UActorComponent>>> InterfaceToComponentInstanceMap;


	void AddToWorld(const FWorldContext& WorldContext,
		const FGameFeatureStateChangeContext& GameFeatureContext, FContextHandles& WorldContextHandles);
	
	void HandleGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext GameFeatureContext);
	void HandleInterfaceChange(UGKInterfaceComponent* InterfaceComponent, FName EventName, const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext);
	
	void TryAddComponentsToInterfacedActor(UGKInterfaceComponent* InterfaceComponent, const FWorldContext& WorldContext);
	void TryRemoveComponentsFromInterfacedActor(UGKInterfaceComponent* InterfaceComponent, const FWorldContext& WorldContext);

	TMap<FGameFeatureStateChangeContext, FContextHandles> ContextHandles;

};
