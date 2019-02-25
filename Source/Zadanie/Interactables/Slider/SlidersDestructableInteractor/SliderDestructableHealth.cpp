// Made by Vladyslav Kulinych.

#include "SliderDestructableHealth.h"
#include "Destructables/Destructable.h"

void ASliderDestructableHealth::OnRep_SliderValue()
{
	// Setting the destructable health to the amount as calculated
	if (Role == ROLE_Authority)
	{
		if (TiedDestructable)
		{
			TiedDestructable->ServerSetInitialHealth(InitialAmountOfHealthMin + (InitialAmountOfHealthMax - InitialAmountOfHealthMin)*SliderValue);
		}
	}
}