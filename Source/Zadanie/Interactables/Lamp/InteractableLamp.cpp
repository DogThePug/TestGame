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

	PointLight = CreateDefaultSubobject<UPointLightComponent>(FName("PointLight"));
	PointLight->SetupAttachment(DefaultMesh);
	PointLight->SetIntensity(LightIntensity);
	PointLight->SetRelativeLocation(FVector(10.f, 0.f, 25.f));
}

void AInteractableLamp::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		LightIntensity = FMath::Clamp(CurrentLightIntensity, 0.f, MaxLightIntensity);
		OnRep_LightIntensity();
	}
}

void AInteractableLamp::ClientBeginHover()
{
	if (bCanBeDirectlyInteractedWith)
	{
		DefaultMesh->SetVectorParameterValueOnMaterials(FName("ColorScreen"), FVector(1.f, 0.5f, 1.f));
	}
}

void AInteractableLamp::ClientEndHover()
{
	DefaultMesh->SetVectorParameterValueOnMaterials(FName("ColorScreen"), FVector(1.f, 0.38f, 0.f));
}

void AInteractableLamp::ServerAddColorToBlend_Implementation(FLinearColor Color)
{
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
	LightColor = Color;
	OnRep_LightColor();
}

bool AInteractableLamp::ServerSetLightColor_Validate(FLinearColor Color)
{
	return true;
}

void AInteractableLamp::IndirectInteract_Implementation()
{
	if (Role == ROLE_Authority)
	{
		SwitchLampState();
	}
}

bool AInteractableLamp::IndirectInteract_Validate()
{
	return true;
}


void AInteractableLamp::ServerSetIntencityPercent_Implementation(float Percent)
{
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

bool AInteractableLamp::ServerSetIntencityPercent_Validate(float Percent)
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
	LightIntensity = bLightOn ? 0.f : CurrentLightIntensity;
	bLightOn = !bLightOn;
	OnStateChanged.Broadcast();
	OnRep_LightIntensity();
}

void AInteractableLamp::OnRep_LightIntensity()
{
	PointLight->SetIntensity(LightIntensity);
}

void AInteractableLamp::OnRep_LightColor()
{
	PointLight->SetLightColor(LightColor);
}

void AInteractableLamp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractableLamp, LightIntensity);
	DOREPLIFETIME(AInteractableLamp, LightColor);
}

