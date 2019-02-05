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

	virtual void OnRep_SliderValue();

	void CalculateCurrentColor();

	FLinearColor CurrentColor;
};
