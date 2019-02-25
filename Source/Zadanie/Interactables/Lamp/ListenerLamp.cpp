// Made by Vladyslav Kulinych.

#include "ListenerLamp.h"

void AListenerLamp::BeginPlay()
{
	Super::BeginPlay();

	// Subscribing to OnStateChanged of the lamp to check if all the connected lamps were turned off
	if (Role == ROLE_Authority)
	{
		// Changing so that we can't be turned on/off by hand
		bCanBeDirectlyInteractedWith = false;
		for (auto Lamp : ListenedToLamps)
		{
			if (Lamp)
			{
				Lamp->OnStateChanged.AddDynamic(this, &AListenerLamp::CheckConditionsMet);
			}
		}
	}
	// Force calling conditions met to find out if we have the right conditions right away
	CheckConditionsMet();
}
void AListenerLamp::CheckConditionsMet()
{
	if (Role == ROLE_Authority)
	{
		for (auto Lamp : ListenedToLamps)
		{
			if (Lamp)
			{
				// stop checking if the light on atleast one of the lamps is on 
				if (Lamp->IsLightOn())
				{
					if (IsLightOn())
					{
						IndirectInteract();
					}
					return;
				}
			}
		}
		// turning on the light
		if (!IsLightOn())
		{
			IndirectInteract();
		}
	}
}