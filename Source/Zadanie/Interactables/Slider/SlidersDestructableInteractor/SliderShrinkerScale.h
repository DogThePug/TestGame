// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/SlidersDestructableInteractor/SliderDestructableInteractor.h"
#include "SliderShrinkerScale.generated.h"

/**
 * Slider that changes initial scale of shrinker 
 */
UCLASS()
class ZADANIE_API ASliderShrinkerScale : public ASliderDestructableInteractor
{
	GENERATED_BODY()
	
protected:
	float InitialScaleAmountMin = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "1.0", ClampMax = "4.0"))
	float InitialScaleAmountMax = 4.f;

	// Change the radius of the damage sharing sphere
	virtual void OnRep_SliderValue() override;
};
