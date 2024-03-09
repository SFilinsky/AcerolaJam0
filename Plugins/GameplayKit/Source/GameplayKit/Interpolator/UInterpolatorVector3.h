#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TInterpolator.h"
#include "UInterpolatorVector3.generated.h"


UCLASS(BlueprintType, ClassGroup = (GameplayKit), meta = (BlueprintSpawnableComponent))
class GAMEPLAYKIT_API UInterpolatorVector3 : public UInterpolatorBase
{
	GENERATED_BODY()

public:

    UInterpolatorVector3();
	
    UInterpolatorVector3(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
    {
        Interpolator =  new TInterpolator<FVector>();
        Interpolator->SetCurrentValue(FVector::ZeroVector);
        Interpolator->SetTargetValue(FVector::ZeroVector);
    }
    
public:
	
	UFUNCTION(BlueprintCallable, Category = "Interpolation")
    void Interpolate(float DeltaTime)
    {
        Interpolator->Interpolate(DeltaTime);
    }

    UFUNCTION(BlueprintCallable, Category = "Interpolation")
    void AddToTargetValue(FVector TargetChange)
    {
        Interpolator->AddToTargetValue(TargetChange);
    }

    UFUNCTION(BlueprintCallable, Category = "Interpolation")
    void SetTargetValue(FVector NewValue)
    {
        Interpolator->SetTargetValue(NewValue);
    }

    UFUNCTION(BlueprintCallable, Category = "Interpolation")
    FVector GetTargetValue() const
    {
        return Interpolator->GetTargetValue();
    }

    UFUNCTION(BlueprintCallable, Category = "Interpolation")
    void SetCurrentValue(FVector NewValue)
    {
        Interpolator->SetCurrentValue(NewValue);
    }

    UFUNCTION(BlueprintCallable, Category = "Interpolation")
    FVector GetCurrentValue() const
    {
        return Interpolator->GetCurrentValue();
    }

    UFUNCTION(BlueprintCallable, Category = "Interpolation")
    void SetInterpolationInterval(float NewInterpolationInterval)
    {
        Interpolator->SetInterpolationInterval(NewInterpolationInterval);
    }
	
	/* State */
	
public:
    
    TInterpolator<FVector>* Interpolator;
	
};
