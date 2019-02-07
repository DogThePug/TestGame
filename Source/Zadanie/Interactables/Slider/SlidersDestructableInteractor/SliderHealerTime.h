// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/SlidersDestructableInteractor/SliderDestructableInteractor.h"
#include "SliderHealerTime.generated.h"

/**
 * 
 */
UCLASS()
class ZADANIE_API ASliderHealerTime : public ASliderDestructableInteractor
{
	GENERATED_BODY()
protected:
	float InitialHealTimeMin = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0166666", ClampMax = "2.0"))
		float InitialHealTimeMax = 2.f;

	// Change the radius of the damage sharing sphere
	virtual void OnRep_SliderValue() override;
};
