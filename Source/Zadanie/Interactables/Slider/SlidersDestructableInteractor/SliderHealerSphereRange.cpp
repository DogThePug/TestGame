// Made by Vladyslav Kulinych.

#include "SliderHealerSphereRange.h"
#include "Destructables/DestructableHealer.h"

void ASliderHealerSphereRange::OnRep_SliderValue()
{
	if (Role == ROLE_Authority)
	{
		ADestructableHealer* DamageHealer = Cast<ADestructableHealer>(TiedDestructable);
		if (DamageHealer)
		{
			DamageHealer->ServerSetHealRadius(InitialHealRangeMin + (InitialHealRangeMax - InitialHealRangeMin)*SliderValue);
		}
	}
}
