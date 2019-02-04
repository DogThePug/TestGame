// Made by Vladyslav Kulinych.

#include "ButtonLampInteractor.h"
#include "../Lamp/InteractableLamp.h"
#include "UnrealNetwork.h"

void AButtonLampInteractor::ButtonEffectOnToggle()
{
	for (auto Lamp : AffectedLamps)
	{
		// Interact with lamp
		if (Lamp)
		{
			Lamp->IndirectInteract();
		}
	}
}

void AButtonLampInteractor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AButtonLampInteractor, AffectedLamps);
}
