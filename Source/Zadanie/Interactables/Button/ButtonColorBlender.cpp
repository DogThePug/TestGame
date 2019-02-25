// Made by Vladyslav Kulinych.

#include "ButtonColorBlender.h"
#include "../Lamp/InteractableLamp.h"
#include "UnrealNetwork.h"


void AButtonColorBlender::ButtonEffectOnToggle()
{
	// Cycling through affected lamps and telling them to blend in the color
	for (auto Lamp : AffectedLamps)
	{
		if (Lamp)
		{
			Lamp->ServerAddColorToBlend(ColorToBlend);
		}
	}
}

void AButtonColorBlender::ButtonEffectOnUntoggle()
{
	// Cycling through affected lamps and telling them to unblend the color
	for (auto Lamp : AffectedLamps)
	{
		if (Lamp)
		{
			Lamp->ServerRemoveColorFromBlend(ColorToBlend);
		}
	}
}

void AButtonColorBlender::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AButtonColorBlender, AffectedLamps);
	DOREPLIFETIME(AButtonColorBlender, ColorToBlend);
}


