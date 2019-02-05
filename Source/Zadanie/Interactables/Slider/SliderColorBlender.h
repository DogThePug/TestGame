// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/InteractableSlider.h"
#include "SliderColorBlender.generated.h"

/**
 * 
 */
UCLASS()
class ZADANIE_API ASliderColorBlender : public AInteractableSlider
{
	GENERATED_BODY()
protected:
	// Array of affected lamps
	UPROPERTY(EditAnywhere, Category = "FunctionalitySetup")
		TArray<class AInteractableLamp*> AffectedLamps;

	// Color which will be blended when we slide the slider
	UPROPERTY(EditAnywhere, Category = "FunctionalitySetup")
	FLinearColor BlendedColor;

	virtual void OnRep_SliderValue();

	// Current color to be blended
	FLinearColor CurrentColor;

	// Previous color to be removed from blend
	FLinearColor PreviousColor;
};
