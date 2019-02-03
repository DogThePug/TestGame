// Made by Vladyslav Kulinych.

#include "ButtonDoorInteractor.h"
#include "UnrealNetwork.h"
#include "RemoteDoor/RemoteDoor.h"

AButtonDoorInteractor::AButtonDoorInteractor()
{
	bIsCheckable = false;
	bCanBeToggledOff = false;
}

void AButtonDoorInteractor::ButtonEffectOnToggle()
{
	if (AffectedDoor)
	{
		AffectedDoor->ServerInitiateStateChange();
	}
}

void AButtonDoorInteractor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
