// Made by Vladyslav Kulinych.

#include "InteractableLamp.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "UnrealNetwork.h"
#include "Components/PointLightComponent.h"
#include "TimerManager.h"

AInteractableLamp::AInteractableLamp()
{
	// Set default mesh 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Meshes/SM_Lamp_Wall"));
	DefaultMesh->SetStaticMesh(MeshAsset.Object);

	// Setup point light
	PointLight = CreateDefaultSubobject<UPointLightComponent>(FName("PointLight"));
	PointLight->SetupAttachment(DefaultMesh);
	PointLight->SetIntensity(LightIntensity);
	PointLight->SetRelativeLocation(FVector(10.f, 0.f, 25.f));
}

void AInteractableLamp::BeginPlay()
{
	Super::BeginPlay();

	// Change light intencity to match the current one, as was set by design
	if (Role == ROLE_Authority)
	{
		LightIntensity = FMath::Clamp(CurrentLightIntensity, 0.f, MaxLightIntensity);
		OnRep_LightIntensity();
	}
}

void AInteractableLamp::ClientBeginHover()
{
	// Changing vector parameter on the lamp to signify that local player has hovered over this lamp
	if (bCanBeDirectlyInteractedWith)
	{
		DefaultMesh->SetVectorParameterValueOnMaterials(FName("ColorScreen"), FVector(1.f, 0.5f, 1.f));
	}
}

void AInteractableLamp::ClientEndHover()
{
	// Changing vector parameter on the lamp to singify that local player no longer hovers over this lamp
	DefaultMesh->SetVectorParameterValueOnMaterials(FName("ColorScreen"), FVector(1.f, 0.38f, 0.f));
}

void AInteractableLamp::ServerAddColorToBlend_Implementation(FLinearColor Color)
{
	// Adding color to the blend and mixing them, changing the color of the lamp
	if (Role == ROLE_Authority)
	{
		ColorBlendArray.Add(Color);
		FLinearColor Blend = FLinearColor(0.f,0.f,0.f,0.f);
		for (auto Color : ColorBlendArray)
		{
			Blend += Color;
		}
		ServerSetLightColor(Blend);
	}
}

bool AInteractableLamp::ServerAddColorToBlend_Validate(FLinearColor Color)
{
	return true;
}

void AInteractableLamp::ServerRemoveColorFromBlend_Implementation(FLinearColor Color)
{
	// Removing color from blend and recalculating the color that has to be set 
	if (Role == ROLE_Authority)
	{
		for (int i = 0; i < ColorBlendArray.Num(); i++)
		{
			if (ColorBlendArray[i] == Color)
			{
				ColorBlendArray.RemoveAt(i);
			}
		}
		ColorBlendArray.Remove(Color);
		FLinearColor Blend = FLinearColor(0.f, 0.f, 0.f, 0.f);
		for (auto Color : ColorBlendArray)
		{
			Blend += Color;
		}
		ServerSetLightColor(Blend);
	}
}

bool AInteractableLamp::ServerRemoveColorFromBlend_Validate(FLinearColor Color)
{
	return true;
}

void AInteractableLamp::ServerSetLightColor_Implementation(FLinearColor Color)
{
	// Setting the color of the lamp
	LightColor = Color;
	OnRep_LightColor();
}

bool AInteractableLamp::ServerSetLightColor_Validate(FLinearColor Color)
{
	return true;
}

void AInteractableLamp::IndirectInteract_Implementation()
{
	// Switching lamp state when we were directly interacted with
	if (Role == ROLE_Authority)
	{
		SwitchLampState();
	}
}

bool AInteractableLamp::IndirectInteract_Validate()
{
	return true;
}


void AInteractableLamp::ServerSetIntensityPercent_Implementation(float Percent)
{
	// Changing the intencity based on the maximum (initial) amount and the percent that came in
	if (Role == ROLE_Authority)
	{
		CurrentLightIntensity = FMath::Clamp(MaxLightIntensity*Percent, 0.f, MaxLightIntensity);
		if (bLightOn)
		{
			LightIntensity = CurrentLightIntensity;
			OnRep_LightIntensity();
		}
	}
}

bool AInteractableLamp::ServerSetIntensityPercent_Validate(float Percent)
{
	return true;
}
bool AInteractableLamp::IsLightOn() const
{
	return bLightOn;
}

void AInteractableLamp::ServerInteractPostCheck()
{
	// Turn the light on/off if we can directly interact with it
	if (bCanBeDirectlyInteractedWith)
	{
		SwitchLampState();
	}
}

void AInteractableLamp::SwitchLampState()
{
	// Turning lamp on or off, considering the current light intencity that was set
	LightIntensity = bLightOn ? 0.f : CurrentLightIntensity;
	bLightOn = !bLightOn;
	OnStateChanged.Broadcast();
	OnRep_LightIntensity();
}

void AInteractableLamp::OnRep_LightIntensity()
{
	// Setting light intensity
	PointLight->SetIntensity(LightIntensity);
}

void AInteractableLamp::OnRep_LightColor()
{
	// Changing light color
	PointLight->SetLightColor(LightColor);
}

void AInteractableLamp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractableLamp, LightIntensity);
	DOREPLIFETIME(AInteractableLamp, LightColor);
}

