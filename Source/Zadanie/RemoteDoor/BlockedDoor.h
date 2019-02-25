// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "RemoteDoor/RemoteDoor.h"
#include "BlockedDoor.generated.h"

/**
 *  Door that can be blocked by Destructable Blockers. Opens only when all the blockers are destroyed.
 */
UCLASS()
class ZADANIE_API ABlockedDoor : public ARemoteDoor
{
	GENERATED_BODY()
	
protected:
	// Array of blockers that once are destroyed will let the door to be opened
	TArray<class ADestructable*> DoorBlockers;

	// Setting a timer to check if we have connected blockers. If not, open the door.
	virtual void BeginPlay() override;
public:
	// Adding door blocker to the array and subscribing to know when it has been destroyed
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerConnectDoorBlocker(class ADestructable* Blocker);

	// Removing blocker from the array and checking if all the blockers were removed from this door
	UFUNCTION()
	void ReactOnBlockerDestroyed(class ADestructable* Blocker);
};
