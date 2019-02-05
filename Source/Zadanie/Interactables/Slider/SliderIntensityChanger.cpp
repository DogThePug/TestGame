// Made by Vladyslav Kulinych.

#include "SliderIntensityChanger.h"
#include "../Lamp/InteractableLamp.h"

void ASliderIntensityChanger::OnRep_SliderValue()
{
	if (Role == ROLE_Authority)
	{
		for (auto Lamp : AffectedLamps)
		{
			if (Lamp)
			{
				Lamp->ServerSetIntencityPercent(SliderValue);
			}
		}
	}
}
