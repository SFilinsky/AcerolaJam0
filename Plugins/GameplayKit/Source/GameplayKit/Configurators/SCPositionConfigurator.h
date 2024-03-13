// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayKit/Overrides/GKSceneComponent.h"
#include "SCPositionConfigurator.generated.h"

class USCPosition;
/**
 * Allows to configure positions in editor
 */
UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class GAMEPLAYKIT_API USCPositionConfigurator : public UGKSceneComponent
{
	GENERATED_BODY()
	
	USCPositionConfigurator();

public:

	TArray<FTransform> GetConfiguredPositions();
	
protected:
	
	virtual void OnRegister() override;
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
	
	#if WITH_EDITOR
		virtual void PostEditComponentMove(bool bFinished);
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
	#endif
	
	void UpdatePositionComponents();
	FName GetPositionComponentName(const int32 Index);

	#if WITH_EDITOR
		void EnsureUniqueID();
		void GenerateUniqueID();
		bool IsUniqueIDNotSet() const;
	
		FString GetUniquePositionComponentID(const int32 Index);

		void CacheTransform(int32 Index, FTransform Transform);
	#endif
	

	/* State */
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Position Settings", meta=(ClampMin="0", UIMin="0", ClampMaxMax="100", UIMax="100"))
	int32 NumberOfPositions = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Position Settings")
	TArray<USCPosition*> PositionComponents;

	
	#if WITH_EDITORONLY_DATA
		UPROPERTY(VisibleAnywhere, Category = "Identification")
	    FString UniqueID;
	#endif
	
	int32 LastNumberOfPositions = NumberOfPositions;
	
	#if WITH_EDITOR
		inline static TMap<FString, FTransform> TransformCache = TMap<FString, FTransform>();
	#endif
	
	
};
