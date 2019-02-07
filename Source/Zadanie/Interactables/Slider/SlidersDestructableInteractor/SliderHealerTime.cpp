// Made by Vladyslav Kulinych.

#include "SliderHealerTime.h"
#include "Destructables/DestructableHealer.h"

void ASliderHealerTime::OnRep_SliderValue()
{
	if (Role == ROLE_Authority)
	{
		ADestructableHealer* DamageHealer = Cast<ADestructableHealer>(TiedDestructable);
		if (DamageHealer)
		{
			DamageHealer->ServerSetHealTime(InitialHealTimeMin + (InitialHealTimeMax - InitialHealTimeMin)*SliderValue);
		}
	}
}
