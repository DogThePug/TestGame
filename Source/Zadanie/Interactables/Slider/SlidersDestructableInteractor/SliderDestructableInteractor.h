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
	// Function that ties destructable to slider
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBindWithDestructable(class ADestructable* Destructable);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	class ADestructable* TiedDestructable;

	// Function to override what happens when we tie the destructable
	virtual void OnDestructableTied();
};
