// Made by Vladyslav Kulinych.

#include "SliderSharerRadius.h"
#include "Destructables/DamageSharer.h"

void ASliderSharerRadius::OnRep_SliderValue()
{
	if (Role == ROLE_Authority)
	{
		ADamageSharer* DamageSharer = Cast<ADamageSharer>(TiedDestructable);
		if (DamageSharer)
		{
			DamageSharer->ServerSetShareRadius(InitialSphereRadiusMin + (InitialSphereRadiusMax - InitialSphereRadiusMin)*SliderValue);
		}
	}
}
