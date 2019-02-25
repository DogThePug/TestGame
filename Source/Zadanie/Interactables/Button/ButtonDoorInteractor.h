// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "InteractableButton.h"
#include "ButtonDoorInteractor.generated.h"

/**
 * Button to interact with door. Opens/Closes it.
 */
UCLASS()
class ZADANIE_API AButtonDoorInteractor : public AInteractableButton
{
	GENERATED_BODY()
public:
	AButtonDoorInteractor();

private:
	// Door which this button is going to close/open
	UPROPERTY(Category = "FunctionalitySetup", EditAnywhere, meta = (BlueprintProtected = "true"))
	class ARemoteDoor* AffectedDoor;

	// Changing the state of door 
	virtual void ButtonEffectOnToggle() override;
};
