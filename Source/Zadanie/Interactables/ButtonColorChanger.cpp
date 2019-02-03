// Made by Vladyslav Kulinych.

#include "ButtonColorChanger.h"
#include "InteractableLamp.h"
#include "UnrealNetwork.h"


AButtonColorChanger::AButtonColorChanger()
{
	ColorToChangeTo = FLinearColor(1.f, 1.f, 1.f, 1.f); // White
}

void AButtonColorChanger::ButtonEffectOnToggle()
{
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
