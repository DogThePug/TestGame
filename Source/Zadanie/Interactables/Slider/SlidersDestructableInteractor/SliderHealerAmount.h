// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/SlidersDestructableInteractor/SliderDestructableInteractor.h"
#include "SliderHealerAmount.generated.h"

/**
 *  Slider that can be tied to destructable healer to change the amount of health that it heals based on the slider value and given range
 */
UCLASS()
class ZADANIE_API ASliderHealerAmount : public ASliderDestructableInteractor
{
	GENERATED_BODY()
	
protected:
	// Initial amount of health restoring that this slider sets to the healer
	float InitialHealAmountMin = 2.f;
	
	// Maximum amount of health that is set when slider value reaches 1.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "2.0", ClampMax = "100.0"))
	float InitialHealAmountMax = 100.f;

	// Change the radius of the damage sharing sphere
	virtual void OnRep_SliderValue() override;
};
