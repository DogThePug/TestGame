// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "InteractableButton.h"
#include "ButtonLampInteractor.generated.h"

/**
 * Button that indirectly interacts (Turns on/off) with lamp(s)
 */
UCLASS()
class ZADANIE_API AButtonLampInteractor : public AInteractableButton
{
	GENERATED_BODY()
protected:
	// Array of lamps that we interract with upon toggling
	UPROPERTY(Category = "FunctionalitySetup", EditAnywhere, meta = (BlueprintProtected = "true"))
	TArray<class AInteractableLamp*> AffectedLamps;

	// Cycling through the lamps and changing their color
	virtual void ButtonEffectOnToggle() override;
};
