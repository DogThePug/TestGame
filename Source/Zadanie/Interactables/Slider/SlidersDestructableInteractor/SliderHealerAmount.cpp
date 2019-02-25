// Made by Vladyslav Kulinych.

#include "SliderHealerAmount.h"
#include "Destructables/DestructableHealer.h"

void ASliderHealerAmount::OnRep_SliderValue()
{
	if (Role == ROLE_Authority)
	{
		// Changing the amount of health healed by healer destructable
		ADestructableHealer* DamageHealer = Cast<ADestructableHealer>(TiedDestructable);
		if (DamageHealer)
		{
			DamageHealer->ServerSetHealAmount(InitialHealAmountMin + (InitialHealAmountMax - InitialHealAmountMin)*SliderValue);
		}
	}
}
