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
	float InitialShareAmountMin = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float InitialShareAmountMax = 0.8f;

	// Change the radius of the damage sharing sphere
	virtual void OnRep_SliderValue() override;
};
