// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/InteractableSlider.h"
#include "SliderIntensityChanger.generated.h"

/**
 *  Slider that changes intensity of a lamp
 */
UCLASS()
class ZADANIE_API ASliderIntensityChanger : public AInteractableSlider
{
	GENERATED_BODY()
	
protected:
	// Array of affected lamps
	UPROPERTY(EditAnywhere, Category = "FunctionalitySetup")
	TArray<class AInteractableLamp*> AffectedLamps;

	virtual void OnRep_SliderValue();
};
