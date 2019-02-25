 // Made by Vladyslav Kulinych.

#include "SliderSharerAmount.h"
#include "Destructables/DamageSharer.h"

void ASliderSharerAmount::OnRep_SliderValue()
{
	if (Role == ROLE_Authority)
	{
		// Change the % of damage that is shared by the DamageSharer
		ADamageSharer* DamageSharer = Cast<ADamageSharer>(TiedDestructable);
		if (DamageSharer)
		{
			DamageSharer->ServerSetShareAmount(InitialShareAmountMin + (InitialShareAmountMax - InitialShareAmountMin)*SliderValue);
		}
	}
}
