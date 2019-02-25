// Made by Vladyslav Kulinych.

#include "SliderIntensityChanger.h"
#include "../Lamp/InteractableLamp.h"

void ASliderIntensityChanger::OnRep_SliderValue()
{
	if (Role == ROLE_Authority)
	{
		// Cycle through all the affected lamps and set their intensity
		for (auto Lamp : AffectedLamps)
		{
			if (Lamp)
			{
				Lamp->ServerSetIntensityPercent(SliderValue);
			}
		}
	}
}
