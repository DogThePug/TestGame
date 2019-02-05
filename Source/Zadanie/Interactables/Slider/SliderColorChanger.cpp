// Made by Vladyslav Kulinych.

#include "SliderColorChanger.h"
#include "../Lamp/InteractableLamp.h"

void ASliderColorChanger::OnRep_SliderValue()
{
	// Calculating and changing color
	CalculateCurrentColor();

	if(Role == ROLE_Authority)
	for (auto Lamp : AffectedLamps)
	{
		if (Lamp)
		{
			Lamp->ServerSetLightColor(CurrentColor);
		}
	}
}

void ASliderColorChanger::CalculateCurrentColor()
{
	// Gradually going from red to green when in the range 0% to 50%, then green to blue when in range 50% to 100%
	if (SliderValue >= 0.f && SliderValue < 0.5f)
	{
		CurrentColor = FLinearColor(sin(1.f-(SliderValue)/0.5f), sin((SliderValue)/0.5f), 0.f);
	}
	else if (SliderValue >= 0.5f && SliderValue <= 1.f)
	{
		CurrentColor = FLinearColor(0.f, sin(1.f-(SliderValue - 0.5f) / 0.5f), sin((SliderValue - 0.5f) / 0.5f));
	}
}
