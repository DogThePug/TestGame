// Made by Vladyslav Kulinych.

#include "SliderHealerAmount.h"
#include "Destructables/DestructableHealer.h"

void ASliderHealerAmount::OnRep_SliderValue()
{
	if (Role == ROLE_Authority)
	{
		ADestructableHealer* DamageHealer = Cast<ADestructableHealer>(TiedDestructable);
		if (DamageHealer)
		{
			DamageHealer->ServerSetHealAmount(InitialHealAmountMin + (InitialHealAmountMax - InitialHealAmountMin)*SliderValue);
		}
	}
}
