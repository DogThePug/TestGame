// Made by Vladyslav Kulinych.

#include "PuzzleRemoteDoor.h"
#include "../Interactables/InteractableLamp.h"
#include "UnrealNetwork.h"

void APuzzleRemoteDoor::BeginPlay()
{
	Super::BeginPlay();

	// Subscribing to OnStateChanged
	if (Role == ROLE_Authority)
	{
		for (auto Lamp : ListenedToLamps)
		{
			if (Lamp)
			{
				Lamp->OnStateChanged.AddDynamic(this, &APuzzleRemoteDoor::CheckConditionsMet);
			}
		}
	}
}

void APuzzleRemoteDoor::CheckConditionsMet()
{
	// Checking if all the lights are off
	if (Role == ROLE_Authority)
	{
		for (auto Lamp : ListenedToLamps)
		{
			if (Lamp)
			{
				// stop checking if the light on atleast one of the lamps is on 
				if (Lamp->IsLightOn()) return;
			}
		}
	}

	// Open the door
	if (!bIsOpen)
	{
		ServerInitiateStateChange();
	}
}

void APuzzleRemoteDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APuzzleRemoteDoor, ListenedToLamps);
}
