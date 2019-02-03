// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableButton.h"
#include "ButtonColorBlender.generated.h"

/**
 * Button to blend colors on the affected lamp(s)
 */
UCLASS()
class ZADANIE_API AButtonColorBlender : public AInteractableButton
{
	GENERATED_BODY()
	
	// Array of lamps that we interract with upon toggling
	UPROPERTY(Replicated, Category = "FunctionalitySetup", EditAnywhere, meta = (BlueprintProtected = "true"))
	TArray<class AInteractableLamp*> AffectedLamps;

	// Color that we will blend for lamp
	UPROPERTY(Replicated, Category = "FunctionalitySetup", EditAnywhere, meta = (BlueprintProtected = "true"))
	FLinearColor ColorToBlend;

	// Cycling through the lamps and changing their color
	virtual void ButtonEffectOnToggle() override;

	// Cycling through the lamps and changing their color
	virtual void ButtonEffectOnUntoggle() override;

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
};
