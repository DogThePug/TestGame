// Made by Vladyslav Kulinych.

#include "Destructable.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "UnrealNetwork.h"

// Sets default values
ADestructable::ADestructable()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bReplicateMovement = true;

	// Seting up default mesh
	DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	DefaultMesh->SetSimulatePhysics(true);
	DefaultMesh->SetNotifyRigidBodyCollision(true);
	DefaultMesh->SetEnableGravity(true);

	RootComponent = DefaultMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Meshes/SM_MatPreviewMesh_02"));

	if (MeshAsset.Object)
	{
		DefaultMesh->SetStaticMesh(MeshAsset.Object);
	}

	// Finding default particles asset for on hit particles
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticlesAsset(TEXT("ParticleSystem'/Game/Particles/P_Explosion_Bomb.P_Explosion_Bomb'"));

	if (HitParticlesAsset.Object)
	{
		OnDeathParticles = HitParticlesAsset.Object;
	}
}

// Called when the game starts or when spawned
void ADestructable::BeginPlay()
{
	Super::BeginPlay();
	
	if (Role == ROLE_Authority)
	{
		Health = InitialHealth;
		OnRep_Health();
	}
}

void ADestructable::SequenceDestroy_Implementation()
{
	if (OnDeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnDeathParticles, GetActorLocation(), GetActorRotation());
	}

	Destroy();
}

void ADestructable::OnRep_Health()
{
	// Update Health amount in text
}

void ADestructable::ServerTakeDamage_Implementation(float Damage)
{
	Health -= Damage;
	OnRep_Health();
	if (Health <= 0)
	{
		SequenceDestroy();
	}
}

bool ADestructable::ServerTakeDamage_Validate(float Damage)
{
	return true;
}

void ADestructable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADestructable, Health);
}

// Called every frame
void ADestructable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

