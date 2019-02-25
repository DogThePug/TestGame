// Made by Vladyslav Kulinych.

#include "BlockedDoor.h"
#include "TimerManager.h"
#include "../Destructables/Destructable.h"

void ABlockedDoor::BeginPlay()
{
	Super::BeginPlay();

	// Check on the server if we have connected blockers after 0.2 seconds. If not, open the door.
	if (Role == ROLE_Authority)
	{
		FTimerHandle THandle;
		FTimerDelegate TDelegate;

		TDelegate.BindLambda([this]() {
			if (DoorBlockers.Num() == 0)
			{
				if (!bIsOpen)
				{
					ServerInitiateStateChange();
				}
			}
		}
		);

		GetWorldTimerManager().SetTimer(THandle, TDelegate, 0.2f, false);
	}
}

void ABlockedDoor::ReactOnBlockerDestroyed(ADestructable * Blocker)
{
	// Remove the blocker from the array
	DoorBlockers.Remove(Blocker);

	// Check if we have no more blockers in the array, open if not
	if (DoorBlockers.Num() == 0)
	{
		if (!bIsOpen)
		{
			ServerInitiateStateChange();
		}
	}
}


void ABlockedDoor::ServerConnectDoorBlocker_Implementation(ADestructable * Blocker)
{
	// Adding the blocker to the array and subscribing to the destruction event 
	DoorBlockers.Add(Blocker);
	Blocker->OnDestroyed.AddDynamic(this, &ABlockedDoor::ReactOnBlockerDestroyed);
}

bool ABlockedDoor::ServerConnectDoorBlocker_Validate(ADestructable * Blocker)
{
	return true;
}