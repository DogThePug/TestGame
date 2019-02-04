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
	UPROPERTY(Replicated, Category = "FunctionalitySetup", EditAnywhere, meta = (BlueprintProtected = "true"))
	class ARemoteDoor* AffectedDoor;

	// Cycling through the lamps and changing their color
	virtual void ButtonEffectOnToggle() override;

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

};
