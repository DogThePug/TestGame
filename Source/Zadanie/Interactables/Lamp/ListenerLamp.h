// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Lamp/InteractableLamp.h"
#include "ListenerLamp.generated.h"

/**
 *  Lamp that listenes to other connected lamps and checks if all of them turned off or not and reacts by turning on the light.
 */
UCLASS()
class ZADANIE_API AListenerLamp : public AInteractableLamp
{
	GENERATED_BODY()
	
protected:
	// Subscribe to the change of state in
	virtual void BeginPlay() override;

private:
	// Array of lamps to listen to. When all of them will be off, this door will open
	UPROPERTY(EditAnywhere, Category = "BehaviorSetup")
		TArray<class AInteractableLamp*> ListenedToLamps;

	// Checking if the conditions to open the door have been met
	UFUNCTION()
	void CheckConditionsMet();
};
