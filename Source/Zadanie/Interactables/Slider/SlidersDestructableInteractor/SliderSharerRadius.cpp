// Made by Vladyslav Kulinych.

#include "SliderSharerRadius.h"
#include "Destructables/DamageSharer.h"

void ASliderSharerRadius::OnRep_SliderValue()
{
	if (Role == ROLE_Authority)
	{
		// Set the radius in which damage sharer shares it's damage
		ADamageSharer* DamageSharer = Cast<ADamageSharer>(TiedDestructable);
		if (DamageSharer)
		{
			DamageSharer->ServerSetShareRadius(InitialSphereRadiusMin + (InitialSphereRadiusMax - InitialSphereRadiusMin)*SliderValue);
		}
	}
}
