// Made by Vladyslav Kulinych.

#include "ButtonDoorInteractor.h"
#include "UnrealNetwork.h"
#include "RemoteDoor/RemoteDoor.h"

AButtonDoorInteractor::AButtonDoorInteractor()
{
	// Making this button uncheckable and untoglable
	bIsCheckable = false;
	bCanBeToggledOff = false;
}

void AButtonDoorInteractor::ButtonEffectOnToggle()
{
	// Initiating the state change on door
	if (AffectedDoor)
	{
		AffectedDoor->ServerInitiateStateChange();
	}
}
