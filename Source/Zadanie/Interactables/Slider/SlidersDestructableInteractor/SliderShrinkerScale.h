// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/SlidersDestructableInteractor/SliderDestructableInteractor.h"
#include "SliderShrinkerScale.generated.h"

/**
 * Slider that changes initial scale of a tied to shrinker 
 */
UCLASS()
class ZADANIE_API ASliderShrinkerScale : public ASliderDestructableInteractor
{
	GENERATED_BODY()
	
protected:
	// Initial scale that is set to the shrinker
	float InitialScaleAmountMin = 1.f;

	// Maximum scale that can be set via slider. Achieved when slider value reaches 1.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "1.0", ClampMax = "4.0"))
	float InitialScaleAmountMax = 4.f;

	// Change the radius of the damage sharing sphere
	virtual void OnRep_SliderValue() override;
};
