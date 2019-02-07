// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "RemoteDoor/RemoteDoor.h"
#include "BlockedDoor.generated.h"

/**
 * 
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
	// Adding door blocker
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerConnectDoorBlocker(class ADestructable* Blocker);

	UFUNCTION()
	void ReactOnBlockerDestroyed(class ADestructable* Blocker);
};
