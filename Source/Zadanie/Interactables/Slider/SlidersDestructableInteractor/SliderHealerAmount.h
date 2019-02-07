// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/SlidersDestructableInteractor/SliderDestructableInteractor.h"
#include "SliderHealerAmount.generated.h"

/**
 * 
 */
UCLASS()
class ZADANIE_API ASliderHealerAmount : public ASliderDestructableInteractor
{
	GENERATED_BODY()
	
protected:
	float InitialHealAmountMin = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "2.0", ClampMax = "100.0"))
	float InitialHealAmountMax = 100.f;

	// Change the radius of the damage sharing sphere
	virtual void OnRep_SliderValue() override;
};
