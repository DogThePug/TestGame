// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/SlidersDestructableInteractor/SliderDestructableInteractor.h"
#include "SliderHealerTime.generated.h"

/**
 *  Slider that can be tied to destructable healer to change the time it takes to do next heal cycle based on the slider value and given range
 */
UCLASS()
class ZADANIE_API ASliderHealerTime : public ASliderDestructableInteractor
{
	GENERATED_BODY()
protected:
	// Time which is initialy sete
	float InitialHealTimeMin = 0.2f;

	// Maximum time that is set when slider value reaches 1.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0166666", ClampMax = "2.0"))
		float InitialHealTimeMax = 2.f;

	// Change the radius of the damage sharing sphere
	virtual void OnRep_SliderValue() override;
};
