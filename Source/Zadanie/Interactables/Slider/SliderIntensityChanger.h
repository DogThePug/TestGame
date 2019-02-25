// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/InteractableSlider.h"
#include "SliderIntensityChanger.generated.h"

/**
 *  Slider that changes intensity of affected lamps
 */
UCLASS()
class ZADANIE_API ASliderIntensityChanger : public AInteractableSlider
{
	GENERATED_BODY()
	
protected:
	// Array of affected lamps
	UPROPERTY(EditAnywhere, Category = "FunctionalitySetup")
	TArray<class AInteractableLamp*> AffectedLamps;

	// Cycling through lamps and changing their intensity
	virtual void OnRep_SliderValue();
};
