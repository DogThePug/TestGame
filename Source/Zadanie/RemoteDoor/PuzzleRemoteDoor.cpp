// Made by Vladyslav Kulinych.

#include "PuzzleRemoteDoor.h"
#include "../Interactables/Lamp/InteractableLamp.h"
#include "UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

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

			if (!bIsOpen)
			{
				GetDoorMesh()->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
			}
			
		}
	}

	// Open the door
	if (!bIsOpen)
	{
		GetDoorMesh()->SetRelativeLocation(FVector(0.f, SlidingDistance, 0.f));
	}
}

void APuzzleRemoteDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APuzzleRemoteDoor, ListenedToLamps);
}
