// Made by Vladyslav Kulinych.

#include "DamageSharer.h"
#include "Engine/World.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/Material.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"


ADamageSharer::ADamageSharer()
{
	// Setting up share sphere
	ShareSphere = CreateDefaultSubobject<USphereComponent>(FName("ShareSphere"));
	ShareSphere->SetupAttachment(RootComponent);
	ShareSphere->SetGenerateOverlapEvents(true);
	ShareSphere->SetSphereRadius(ShareRadius);

	// Finding and setting mesh 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Meshes/SM_Shape_Cube.SM_Shape_Cube'"));

	if (MeshAsset.Object)
	{
		DefaultMesh->SetStaticMesh(MeshAsset.Object);
	}

	// Finding and setting material
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("Material'/Game/Materials/M_Metal_Gold.M_Metal_Gold'"));

	if (MaterialAsset.Object)
	{
		DefaultMesh->SetMaterial(0, MaterialAsset.Object);
	}

	// Finding and setting particles
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticlesAsset(TEXT("ParticleSystem'/Game/Particles/P_OnShareArc.P_OnShareArc'"));

	if (ParticlesAsset.Object)
	{
		ParticlesOnShare = ParticlesAsset.Object;
	}

	// Aligning health text in a proper place
	HealthText->SetRelativeLocation(FVector(0.f, 11.f, 133.f));
}

void ADamageSharer::ServerSetShareRadius_Implementation(float Radius)
{
	// Setting radius variable and changing it on the sphere
	ShareRadius = Radius;
	ShareSphere->SetSphereRadius(ShareRadius);
}


bool ADamageSharer::ServerSetShareRadius_Validate(float Radius)
{
	return true;
}

void ADamageSharer::ServerSetShareAmount_Implementation(float Amount)
{
	// Setting % of damage shared
	ShareAmount = FMath::Clamp(Amount, 0.f, 1.f);
}

bool ADamageSharer::ServerSetShareAmount_Validate(float Amount)
{
	return true;
}
void ADamageSharer::ServerTakeDamage(float Damage)
{
	// Sharing the damage before taking it ourselfs
	ShareDamage(Damage);
	
	Super::ServerTakeDamage(Damage);
}

void ADamageSharer::ShareDamage(float& Damage)
{
	// Finding actors that overlap with the share sphere,
	TArray<AActor*> OverlappingActors;
	ShareSphere->GetOverlappingActors(OverlappingActors);

	TArray<ADestructable*> DestructablesHit;
	for (auto Actor : OverlappingActors)
	{
		//filtering out destructables equally 
		ADestructable* PossibleDestructable = Cast<ADestructable>(Actor);
		if (PossibleDestructable && PossibleDestructable != this && !Cast<ADamageSharer>(PossibleDestructable))
		{
			DestructablesHit.Add(PossibleDestructable);
		}
	}

	// sharing the amount of damage to every one
	if (DestructablesHit.Num() != 0)
	{
		float DamageToShare = Damage * ShareAmount;
		Damage -= DamageToShare;

		float DamageTakenPerDestructable = DamageToShare / DestructablesHit.Num();
		for (auto Destructable : DestructablesHit)
		{
			SpawnEmmiterTo(Destructable);
		
			Destructable->ServerTakeDamage(DamageTakenPerDestructable);
		}
	}
}


void ADamageSharer::SpawnEmmiterTo_Implementation(ADestructable * Destructable)
{
	// Spawning emmiter and attaching it's beam source and target points
	if (Destructable)
	{
		UParticleSystemComponent* ParticlesOnDamageShare = UGameplayStatics::SpawnEmitterAttached(ParticlesOnShare, RootComponent, NAME_None, GetActorLocation(), GetActorRotation());
		ParticlesOnDamageShare->SetBeamSourcePoint(0, GetActorLocation(), 0);
		ParticlesOnDamageShare->SetBeamTargetPoint(0, Destructable->GetActorLocation(), 0);
	}
}
