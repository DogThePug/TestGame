// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableButton.h"
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
	UPROPERTY(Replicated, Category = "FunctionalitySetup", EditAnywhere, meta = (BlueprintProtected = "true"))
	TArray<class AInteractableLamp*> AffectedLamps;

	// Cycling through the lamps and changing their color
	virtual void ButtonEffectOnToggle() override;

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
};
