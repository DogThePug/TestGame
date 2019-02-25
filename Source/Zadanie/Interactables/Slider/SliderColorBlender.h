// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/InteractableSlider.h"
#include "SliderColorBlender.generated.h"

/**
 *  Slider that ties to lamps and blends collors based on current slider value
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

	// Unblending old color on the affected lamps and blending in new
	virtual void OnRep_SliderValue();

	// Current color to be blended
	FLinearColor CurrentColor;

	// Previous color to be removed from blend
	FLinearColor PreviousColor;
};
