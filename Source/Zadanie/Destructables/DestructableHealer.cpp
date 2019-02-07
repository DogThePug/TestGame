// Made by Vladyslav Kulinych.

#include "DestructableHealer.h"
#include "Engine/World.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/Material.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"
#include "Materials/MaterialInstance.h"
#include "TimerManager.h"

ADestructableHealer::ADestructableHealer()
{
	HealSphere = CreateDefaultSubobject<USphereComponent>(FName("ShareSphere"));
	HealSphere->SetupAttachment(RootComponent);
	HealSphere->SetGenerateOverlapEvents(true);
	HealSphere->SetSphereRadius(HealRadius);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Meshes/Attenuation_RoundedBox_01.Attenuation_RoundedBox_01'"));

	if (MeshAsset.Object)
	{
		DefaultMesh->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialAsset(TEXT("MaterialInstanceConstant'/Game/Materials/M_LayoutMat_Inst3.M_LayoutMat_Inst3'"));

	if (MaterialAsset.Object)
	{
		DefaultMesh->SetMaterial(0, MaterialAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticlesAsset(TEXT("ParticleSystem'/Game/Particles/P_OnHealArc.P_OnHealArc'"));

	if (ParticlesAsset.Object)
	{
		ParticlesOnHeal = ParticlesAsset.Object;
	}

	HealthText->SetRelativeLocation(FVector(0.f, 11.f, 133.f));
}

void ADestructableHealer::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		GetWorldTimerManager().SetTimer(HealTimerHandle, this, &ADestructableHealer::HealDamage, HealTime, false);
	}
}


void ADestructableHealer::ServerSetHealRadius_Implementation(float Radius)
{
	HealRadius = Radius;
	HealSphere->SetSphereRadius(HealRadius);
}

bool ADestructableHealer::ServerSetHealRadius_Validate(float Radius)
{
	return true;
}

void ADestructableHealer::ServerSetHealAmount_Implementation(float Amount)
{
	HealAmount = FMath::Clamp(Amount, 0.f, 100.f);
}

bool ADestructableHealer::ServerSetHealAmount_Validate(float Amount)
{
	return true;
}

void ADestructableHealer::ServerSetHealTime_Implementation(float Time)
{
	HealTime = Time;
}

bool ADestructableHealer::ServerSetHealTime_Validate(float Time)
{
	return true;
}

void ADestructableHealer::HealDamage()
{
	TArray<AActor*> OverlappingActors;
	HealSphere->GetOverlappingActors(OverlappingActors);

	TArray<ADestructable*> DestructablesHit;
	for (auto Actor : OverlappingActors)
	{
		ADestructable* PossibleDestructable = Cast<ADestructable>(Actor);
		if (PossibleDestructable && PossibleDestructable != this)
		{
			DestructablesHit.Add(PossibleDestructable);
		}
	}

	if(DestructablesHit.Num() != 0)
	{
		for (auto Destructable : DestructablesHit)
		{
			SpawnEmmiterTo(Destructable);

			Destructable->ServerHealDamage(HealAmount);
		}
	}


	GetWorldTimerManager().SetTimer(HealTimerHandle, this, &ADestructableHealer::HealDamage, HealTime, false);
}

void ADestructableHealer::SpawnEmmiterTo_Implementation(ADestructable * Destructable)
{
	if (Destructable)
	{
		UParticleSystemComponent* ParticlesOnDamageHeal = UGameplayStatics::SpawnEmitterAttached(ParticlesOnHeal, RootComponent, NAME_None, GetActorLocation(), GetActorRotation());
		ParticlesOnDamageHeal->SetBeamSourcePoint(0, GetActorLocation(), 0);
		ParticlesOnDamageHeal->SetBeamTargetPoint(0, Destructable->GetActorLocation(), 0);
	}
}
