// Made by Vladyslav Kulinych.

#include "SliderDestructableInteractor.h"
#include "Destructables/Destructable.h"


void ASliderDestructableInteractor::ServerBindWithDestructable_Implementation(ADestructable* Destructable)
{
	if (Destructable)
	{
		TiedDestructable = Destructable;
		OnDestructableTied();
	}
}

bool ASliderDestructableInteractor::ServerBindWithDestructable_Validate(ADestructable* Destructable)
{
	return true;
}

void ASliderDestructableInteractor::OnDestructableTied()
{
	OnRep_SliderValue();
}
