// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/InteractableSlider.h"
#include "SliderDestructableInteractor.generated.h"

/**
 * Slider that can be tied with a destructable. Effect on the destructables is implemented by children of this class
 */
UCLASS()
class ZADANIE_API ASliderDestructableInteractor : public AInteractableSlider
{
	GENERATED_BODY()

public:
	// Handles tying of this slider to a destructable. See AShootingRangeVolume.
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBindWithDestructable(class ADestructable* Destructable);

protected:
	// Destructable that we are currently tied with 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	class ADestructable* TiedDestructable;

	// Function to override what happens when we tie the destructable
	virtual void OnDestructableTied();
};
