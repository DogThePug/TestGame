// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/InteractableSlider.h"
#include "SliderColorChanger.generated.h"

/**
 *  Slider that changes color of the lamp r->g->b
 */
UCLASS()
class ZADANIE_API ASliderColorChanger : public AInteractableSlider
{
	GENERATED_BODY()
	
protected:
	// Array of affected lamps
	UPROPERTY(EditAnywhere, Category = "FunctionalitySetup")
	TArray<class AInteractableLamp*> AffectedLamps;

	// Gradually changing color on affected lamps
	virtual void OnRep_SliderValue();

	// Calculating color basad on the current value of slider
	void CalculateCurrentColor();

	// Collor to be set
	FLinearColor CurrentColor;
};
