// Fill out your copyright notice in the Description page of Project Settings.


#include "SCPositionConfigurator.h"

#include "SCPosition.h"
#include "GameplayKit/HelperFunctions/MapArray.h"

USCPositionConfigurator::USCPositionConfigurator()
{
	// for(int32 i = 0; i < NumberOfPositions; i++)
	// {
	// 	// Construct a name for each point
	// 	FName PointName = *FString::Printf(TEXT("Position %d"), i);
	// 	USceneComponent* NewPoint = CreateDefaultSubobject<USceneComponent>(PointName);
	//
	// 	// Set relative location if necessary or leave it to be positioned in editor
	// 	NewPoint->SetRelativeLocation(FVector::ZeroVector);
	// 	NewPoint->SetRelativeRotation(FRotator::ZeroRotator);
	//
	// 	// Setup the point's attachment to the component
	// 	NewPoint->SetupAttachment(this);
	// 	// if (!NewPoint->IsRegistered())
	// 	// {
	// 	// 	NewPoint->RegisterComponent();
	// 	// }
	// 	// NewPoint->SetVisibility(true);
	//
	// 	// Add the new point to the array
	// 	Positions.Add(NewPoint);
	// }
}

TArray<FTransform> USCPositionConfigurator::GetConfiguredPositions()
{
	return MapArray(PositionComponents, [](const auto PositionComponent)
	{
		return PositionComponent->GetComponentTransform();
	});
}

void USCPositionConfigurator::OnRegister()
{
	Super::OnRegister();
	#if WITH_EDITOR
		EnsureUniqueID();
	#endif
	
	UpdatePositionComponents();
}

void USCPositionConfigurator::PostInitProperties()
{
	Super::PostInitProperties();
	#if WITH_EDITOR
		EnsureUniqueID();
	#endif
}

void USCPositionConfigurator::PostLoad()
{
	Super::PostLoad();
	#if WITH_EDITOR
		EnsureUniqueID();
	#endif	
}

	#if WITH_EDITOR
void USCPositionConfigurator::PostEditComponentMove(bool bFinished)
{
	Super::PostEditComponentMove(bFinished);
}

void USCPositionConfigurator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(USCPositionConfigurator, NumberOfPositions))
	{
		UpdatePositionComponents();
	}
}
#endif

void USCPositionConfigurator::UpdatePositionComponents()
{
	if (LastNumberOfPositions == NumberOfPositions)
	{
		return;
	}
	LastNumberOfPositions = NumberOfPositions;
	
	
	// Clear existing components
	for (int32 Index = PositionComponents.Num() - 1; Index > NumberOfPositions; Index--)
	{
		USceneComponent* PositionComponent = PositionComponents[Index];
		if (PositionComponent && PositionComponent->IsValidLowLevel())
		{
			PositionComponent->UnregisterComponent();
			PositionComponent->DestroyComponent();
			PositionComponents.RemoveAt(Index);
			
			#if WITH_EDITOR
				TransformCache.Remove(GetUniquePositionComponentID(Index));
			#endif
		}
	}

	
	for(int32 Index = PositionComponents.Num(); Index < NumberOfPositions; Index++)
	{
		const FName PointName = GetPositionComponentName(Index);
		USCPosition* NewPositionComponent = NewObject<USCPosition>(this, PointName);
		NewPositionComponent->SetupAttachment(this);
	
		FTransform NewTransform = FTransform();
		
		#if WITH_EDITOR
			const auto CachedTransform = TransformCache.Find(GetUniquePositionComponentID(Index));
			if (CachedTransform != nullptr)
			{
				NewTransform = *CachedTransform;
			}
	
			/** Update cache */
			CacheTransform(Index, NewTransform);
			NewPositionComponent->OnMoved.AddLambda([Index, NewPositionComponent, this]()
			{
				this->CacheTransform(Index, NewPositionComponent->GetRelativeTransform());
			});
		#endif
		NewPositionComponent->SetRelativeTransform(NewTransform);
		
		NewPositionComponent->RegisterComponent();
	
		PositionComponents.Add(NewPositionComponent);
	}

	#if WITH_EDITOR
		const auto Owner = GetOwner();
		if (Owner != nullptr)
		{
			Owner->ReregisterAllComponents();
			
			if (GEngine)
			{
				GEngine->BroadcastLevelActorListChanged();
				if (Owner && !Owner->IsTemplate())
				{
					Owner->MarkPackageDirty();
				}
			}
		}

	#endif
}

FName USCPositionConfigurator::GetPositionComponentName(const int32 Index)
{
	return *FString::Printf(TEXT("Position %d"), Index);
}


#if WITH_EDITOR

	void USCPositionConfigurator::EnsureUniqueID()
	{
		if (IsUniqueIDNotSet())
		{
			GenerateUniqueID();
		}
	}

	void USCPositionConfigurator::GenerateUniqueID()
	{
		UniqueID = FGuid::NewGuid().ToString();
	}

	bool USCPositionConfigurator::IsUniqueIDNotSet() const
	{
		return UniqueID.IsEmpty();
	}

	FString USCPositionConfigurator::GetUniquePositionComponentID(const int32 Index)
	{
		return UniqueID + GetPositionComponentName(Index).ToString();
	}

	void USCPositionConfigurator::CacheTransform(const int32 Index, FTransform Transform)
	{
		const auto CacheId = GetUniquePositionComponentID(Index);
		TransformCache.Add(CacheId, Transform);
	}
#endif

