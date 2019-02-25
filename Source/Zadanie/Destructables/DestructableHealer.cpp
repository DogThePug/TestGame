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
	// Setting up healing sphere
	HealSphere = CreateDefaultSubobject<USphereComponent>(FName("ShareSphere"));
	HealSphere->SetupAttachment(RootComponent);
	HealSphere->SetGenerateOverlapEvents(true);
	HealSphere->SetSphereRadius(HealRadius);

	// Finding mesh asset and setting our default mesh to it
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Meshes/Attenuation_RoundedBox_01.Attenuation_RoundedBox_01'"));

	if (MeshAsset.Object)
	{
		DefaultMesh->SetStaticMesh(MeshAsset.Object);
	}

	// Finding material and setting material of our default mesh to it
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialAsset(TEXT("MaterialInstanceConstant'/Game/Materials/M_LayoutMat_Inst3.M_LayoutMat_Inst3'"));

	if (MaterialAsset.Object)
	{
		DefaultMesh->SetMaterial(0, MaterialAsset.Object);
	}

	// Finding particles that will be spawned when this destructable heals another 
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticlesAsset(TEXT("ParticleSystem'/Game/Particles/P_OnHealArc.P_OnHealArc'"));

	if (ParticlesAsset.Object)
	{
		ParticlesOnHeal = ParticlesAsset.Object;
	}

	// Setting health text relative position to visually match the size of mesh
	HealthText->SetRelativeLocation(FVector(0.f, 11.f, 133.f));
}

void ADestructableHealer::BeginPlay()
{
	Super::BeginPlay();

	// Setting timer to start healing 
	if (Role == ROLE_Authority)
	{
		GetWorldTimerManager().SetTimer(HealTimerHandle, this, &ADestructableHealer::HealDamage, HealTime, false);
	}
}


void ADestructableHealer::ServerSetHealRadius_Implementation(float Radius)
{
	// Setting current radius and changing healing volume to match
	HealRadius = Radius;
	HealSphere->SetSphereRadius(HealRadius);
}

bool ADestructableHealer::ServerSetHealRadius_Validate(float Radius)
{
	return true;
}

void ADestructableHealer::ServerSetHealAmount_Implementation(float Amount)
{
	// Setting how much healing is done
	HealAmount = FMath::Clamp(Amount, 0.f, 100.f);
}

bool ADestructableHealer::ServerSetHealAmount_Validate(float Amount)
{
	return true;
}

void ADestructableHealer::ServerSetHealTime_Implementation(float Time)
{
	// Setting delay before heals
	HealTime = Time;
}

bool ADestructableHealer::ServerSetHealTime_Validate(float Time)
{
	return true;
}

void ADestructableHealer::HealDamage()
{
	// Find all overlapping actors in the vicinity of our healing volume
	TArray<AActor*> OverlappingActors;
	HealSphere->GetOverlappingActors(OverlappingActors);

	// Filter out destructables
	TArray<ADestructable*> DestructablesHit;
	for (auto Actor : OverlappingActors)
	{
		ADestructable* PossibleDestructable = Cast<ADestructable>(Actor);
		if (PossibleDestructable && PossibleDestructable != this)
		{
			DestructablesHit.Add(PossibleDestructable);
		}
	}

	// Heal all destructibles in vicinity
	if(DestructablesHit.Num() != 0)
	{
		for (auto Destructable : DestructablesHit)
		{
			SpawnEmmiterTo(Destructable);

			Destructable->ServerHealDamage(HealAmount);
		}
	}

	// Reset the timer
	GetWorldTimerManager().SetTimer(HealTimerHandle, this, &ADestructableHealer::HealDamage, HealTime, false);
}

void ADestructableHealer::SpawnEmmiterTo_Implementation(ADestructable * Destructable)
{
	// Spawning emmiter and attaching it's beam source and target points
	if (Destructable)
	{
		UParticleSystemComponent* ParticlesOnDamageHeal = UGameplayStatics::SpawnEmitterAttached(ParticlesOnHeal, RootComponent, NAME_None, GetActorLocation(), GetActorRotation());
		ParticlesOnDamageHeal->SetBeamSourcePoint(0, GetActorLocation(), 0);
		ParticlesOnDamageHeal->SetBeamTargetPoint(0, Destructable->GetActorLocation(), 0);
	}
}
