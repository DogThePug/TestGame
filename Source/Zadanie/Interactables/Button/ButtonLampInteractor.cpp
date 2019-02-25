// Made by Vladyslav Kulinych.

#include "ButtonLampInteractor.h"
#include "../Lamp/InteractableLamp.h"
#include "UnrealNetwork.h"

void AButtonLampInteractor::ButtonEffectOnToggle()
{
	// Cycling through affected lamps and indirectly interacting with them
	for (auto Lamp : AffectedLamps)
	{
		if (Lamp)
		{
			Lamp->IndirectInteract();
		}
	}
}


