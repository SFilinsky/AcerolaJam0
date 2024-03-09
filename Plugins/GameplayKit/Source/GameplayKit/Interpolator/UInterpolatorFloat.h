#pragma once

#include "CoreMinimal.h"
#include "TInterpolator.h"
#include "UObject/Object.h"
#include "UInterpolatorFloat.generated.h"

UCLASS(BlueprintType, ClassGroup = (GameplayKit), meta = (BlueprintSpawnableComponent))
class GAMEPLAYKIT_API UInterpolatorFloat : public UInterpolatorBase
{
	GENERATED_BODY()

public:

    UInterpolatorFloat();
	
    UInterpolatorFloat(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
    {
        Interpolator = new TInterpolator<float>();
        Interpolator->SetCurrentValue(0.0f);
        Interpolator->SetTargetValue(0.0f);
    }
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Interpolation")
    void Interpolate(float DeltaTime)
    {
        Interpolator->Interpolate(DeltaTime);
    }

    UFUNCTION(BlueprintCallable, Category = "Interpolation")
    void AddToTargetValue(float TargetChange)
    {
        Interpolator->AddToTargetValue(TargetChange);
    }

    UFUNCTION(BlueprintCallable, Category = "Interpolation")
    void SetTargetValue(float NewValue)
    {
        Interpolator->SetTargetValue(NewValue);
    }

    UFUNCTION(BlueprintCallable, Category = "Interpolation")
    float GetTargetValue() const
    {
        return Interpolator->GetTargetValue();
    }

    UFUNCTION(BlueprintCallable, Category = "Interpolation")
    void SetCurrentValue(float NewValue)
    {
        Interpolator->SetCurrentValue(NewValue);
    }

    UFUNCTION(BlueprintCallable, Category = "Interpolation")
    float GetCurrentValue() const
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

    TInterpolator<float>* Interpolator;
	
};
