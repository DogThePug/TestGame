// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/SlidersDestructableInteractor/SliderDestructableInteractor.h"
#include "SliderHealerSphereRange.generated.h"

/**
 *  Slider that can be tied to destructable healer to change the range in which it heals based on the slider value and given range
 */
UCLASS()
class ZADANIE_API ASliderHealerSphereRange : public ASliderDestructableInteractor
{
	GENERATED_BODY()
protected:
	// Initial range that is set by slider to the healer
	float InitialHealRangeMin = 350.f;

	// Maximum range that is set by slider to the healer. Achieved when the slider value reaches 1.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "350.0", ClampMax = "750.0"))
		float InitialHealRangeMax = 750.f;

	// Change the radius of the damage sharing sphere
	virtual void OnRep_SliderValue() override;
};
