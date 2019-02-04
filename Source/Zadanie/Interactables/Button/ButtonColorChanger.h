// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "InteractableButton.h"
#include "ButtonColorChanger.generated.h"

/**
 *  Button that ties to lamp(s) and changes it's light color to another
 */
UCLASS()
class ZADANIE_API AButtonColorChanger : public AInteractableButton
{
	GENERATED_BODY()

public:
	AButtonColorChanger();

protected:
	// Array of lamps that we interract with upon toggling
	UPROPERTY(Replicated, Category = "FunctionalitySetup", EditAnywhere, meta = (BlueprintProtected = "true"))
	TArray<class AInteractableLamp*> AffectedLamps;

	// Color that we will set affected lamps to
	UPROPERTY(Replicated, Category = "FunctionalitySetup", EditAnywhere, meta = (BlueprintProtected = "true"))
	FLinearColor ColorToChangeTo;

	// Cycling through the lamps and changing their color
	virtual void ButtonEffectOnToggle() override;

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
};
