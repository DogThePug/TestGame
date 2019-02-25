// Made by Vladyslav Kulinych.

#include "ButtonColorChanger.h"
#include "../Lamp/InteractableLamp.h"
#include "UnrealNetwork.h"


AButtonColorChanger::AButtonColorChanger()
{
	// Seting the default value for the changing of color on a lamp
	ColorToChangeTo = FLinearColor(1.f, 1.f, 1.f, 1.f); // White
}

void AButtonColorChanger::ButtonEffectOnToggle()
{
	// Cycling through the affected lamps and changing the color on them
	for (auto Lamp : AffectedLamps)
	{
		if (Lamp)
		{
			Lamp->ServerSetLightColor(ColorToChangeTo);
		}
	}
}

void AButtonColorChanger::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AButtonColorChanger, ColorToChangeTo);
	DOREPLIFETIME(AButtonColorChanger, AffectedLamps);
}
