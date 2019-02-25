// Made by Vladyslav Kulinych.

#include "SliderDestructableInteractor.h"
#include "Destructables/Destructable.h"

void ASliderDestructableInteractor::ServerBindWithDestructable_Implementation(ADestructable* Destructable)
{
	// Tying to the given destructable and force calling the slider change
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
