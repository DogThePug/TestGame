// Made by Vladyslav Kulinych.

#include "SliderShrinkerScale.h"
#include "Destructables/ShrinkingDestructable.h"

void ASliderShrinkerScale::OnRep_SliderValue()
{
	if (Role == ROLE_Authority)
	{
		AShrinkingDestructable* Shrinker = Cast<AShrinkingDestructable>(TiedDestructable);
		if (Shrinker)
		{
			Shrinker->ServerSetInitialScale(FVector(InitialScaleAmountMin + (InitialScaleAmountMax - InitialScaleAmountMin)*SliderValue));
		}
	}
}
