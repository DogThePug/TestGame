// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Button/InteractableButton.h"
#include "VolumeResetButton.generated.h"

/**
 *  Button that resets tied to shooting range volume
 */
UCLASS()
class ZADANIE_API AVolumeResetButton : public AInteractableButton
{
	GENERATED_BODY()
	
protected:
	AVolumeResetButton();

	// Shootin Range Volume that this button will reset on toggling
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FunctionalitySetup")
	class AShootingRangeVolume* ShootingRange;

	// Reseting the range
	virtual void ButtonEffectOnToggle() override;
};
