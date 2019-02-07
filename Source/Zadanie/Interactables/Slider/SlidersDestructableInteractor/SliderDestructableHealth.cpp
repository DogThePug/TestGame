// Made by Vladyslav Kulinych.

#include "SliderDestructableHealth.h"
#include "Destructables/Destructable.h"

void ASliderDestructableHealth::OnRep_SliderValue()
{
	if (Role == ROLE_Authority)
	{
		if (TiedDestructable)
		{
			TiedDestructable->ServerSetInitialHealth(InitialAmountOfHealthMin + (InitialAmountOfHealthMax - InitialAmountOfHealthMin)*SliderValue);
		}
	}
}