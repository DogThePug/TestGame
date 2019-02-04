// Made by Vladyslav Kulinych.

#include "ButtonColorBlender.h"
#include "../Lamp/InteractableLamp.h"
#include "UnrealNetwork.h"


void AButtonColorBlender::ButtonEffectOnToggle()
{
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


