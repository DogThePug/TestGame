// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/SlidersDestructableInteractor/SliderDestructableInteractor.h"
#include "SliderDestructableHealth.generated.h"

/**
 *  Slider that changes the amount of health on the destructable, depending on the slider value and given range.
 */
UCLASS()
class ZADANIE_API ASliderDestructableHealth : public ASliderDestructableInteractor
{
	GENERATED_BODY()
	
protected:
	float InitialAmountOfHealthMin = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "150.0", ClampMax = "250.0"))
	float InitialAmountOfHealthMax = 200.f;

	// Change the amount of initial health on destructable
	virtual void OnRep_SliderValue() override;
};
