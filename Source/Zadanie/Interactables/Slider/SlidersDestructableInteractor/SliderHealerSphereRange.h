// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/SlidersDestructableInteractor/SliderDestructableInteractor.h"
#include "SliderHealerSphereRange.generated.h"

/**
 * 
 */
UCLASS()
class ZADANIE_API ASliderHealerSphereRange : public ASliderDestructableInteractor
{
	GENERATED_BODY()
protected:
	float InitialHealRangeMin = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0166666", ClampMax = "2.0"))
		float InitialHealRangeMax = 750.f;

	// Change the radius of the damage sharing sphere
	virtual void OnRep_SliderValue() override;
};
