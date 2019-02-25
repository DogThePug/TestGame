// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/SlidersDestructableInteractor/SliderDestructableInteractor.h"
#include "SliderSharerAmount.generated.h"

/**
 *  Slider that changes the amount of damage that damage sharer shares
 */
UCLASS()
class ZADANIE_API ASliderSharerAmount : public ASliderDestructableInteractor
{
	GENERATED_BODY()
protected:
	// Initial amount of damage shared
	float InitialShareAmountMin = 0.1f;

	// Maximum amount of damage that can be set. Achieved when the slider value reaches 1.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float InitialShareAmountMax = 0.8f;

	// Change the radius of the damage sharing sphere
	virtual void OnRep_SliderValue() override;
};
