// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Destructables/Destructable.h"
#include "DoorBlocker.generated.h"

/**
 * Destructable that blocks the door. Once all the DoorBlockers are destroyed, the door will open.
 */
UCLASS()
class ZADANIE_API ADoorBlocker : public ADestructable
{
	GENERATED_BODY()
	
public:
	ADoorBlocker();

protected:
	// Door that this actor is blocking
	UPROPERTY(EditAnywhere, Category = "Setup")
	class ABlockedDoor* BlockedDoor;

	//Notifying the door that we are blocking it
	virtual void BeginPlay() override;
};
