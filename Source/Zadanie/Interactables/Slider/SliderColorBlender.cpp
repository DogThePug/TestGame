// Made by Vladyslav Kulinych.

#include "SliderColorBlender.h"
#include "../Lamp/InteractableLamp.h"

void ASliderColorBlender::OnRep_SliderValue()
{
	if (Role == ROLE_Authority)
	{
		// Cycling through each affected lamp and telling it to unblend our previous collor and blend current
		for (auto Lamp : AffectedLamps)
		{
			// Remove previous color from lamp blend
			Lamp->ServerRemoveColorFromBlend(PreviousColor);
			
			// Calculate and add blend of the current color
			CurrentColor = SliderValue * BlendedColor;
			Lamp->ServerAddColorToBlend(CurrentColor);

			// Set our previous color to the current one to remove on the next interation
			PreviousColor = CurrentColor;
		}
	}
}
