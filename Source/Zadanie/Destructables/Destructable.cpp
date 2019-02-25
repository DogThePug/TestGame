// Made by Vladyslav Kulinych.

#include "Destructable.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Materials/Material.h"
#include "TimerManager.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

#include "UnrealNetwork.h"

// Sets default values
ADestructable::ADestructable()
{
	// We need tick to align health text to the local player
	PrimaryActorTick.bCanEverTick = true;

	// Network setup
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
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DeathParticlesAsset(TEXT("ParticleSystem'/Game/Particles/P_Explosion_Bomb.P_Explosion_Bomb'"));

	if (DeathParticlesAsset.Object)
	{
		OnDeathParticles = DeathParticlesAsset.Object;
	}

	// Finding material to apply when we are hurt
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> HurtMaterialAsset(TEXT("MaterialInstanceConstant'/Game/Materials/MI_ShapeInstance.MI_ShapeInstance'"));

	if (HurtMaterialAsset.Object)
	{
		HurtMaterial = (UMaterial*)HurtMaterialAsset.Object;
	}

	HealthText = CreateDefaultSubobject<UTextRenderComponent>(FName("HealthText"));
	HealthText->SetupAttachment(RootComponent);
	HealthText->SetText(TEXT("TestText"));
	HealthText->SetVisibility(false);
	HealthText->SetRelativeLocation(FVector(0.f, 0.f, 274.f));
}



// Called when the game starts or when spawned
void ADestructable::BeginPlay()
{
	Super::BeginPlay();
	
	// Setting health equal to initial health
	if (Role == ROLE_Authority)
	{
		Health = InitialHealth;
	}
	// Both server and client should refresh the text
	OnRep_Health();


	// Getting initial materials of the destructable to later properely apply damage material
	InitialMaterials = DefaultMesh->GetMaterials();

	// Check if health should be visible or not.
	if (bIsHealthVisible)
	{
		// Set visibility to true and update relative transform of text to properely match during runtime
		HealthText->SetVisibility(true);
		HealthTextRelativePosition = HealthText->GetRelativeTransform().GetLocation();
	}
	else
	{
		// Destroy health text and deactivate tick because we don't need it if we don't show HP
		HealthText->DestroyComponent();
		SetActorTickEnabled(false);
	}
}

void ADestructable::SequenceDestroy_Implementation()
{
	// Releasing particles on place of death before actually destroying the actor
	if (OnDeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnDeathParticles, GetActorLocation(), GetActorRotation());
	}

	Destroy();
}

// Called every frame
void ADestructable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Aligning text to face towards local player
	if (HealthText)
	{
		ClientAlignTextToFacePlayer();
	}
}
void ADestructable::SequenceHurt_Implementation()
{
	// Finding the amount of materials on our actor to change them all to hurt material and back
	int MaxIndex = InitialMaterials.Num(); 

	// Change all materials to hurt material
	for (int i = 0; i < MaxIndex; i++)
	{
		DefaultMesh->SetMaterial(i, HurtMaterial);
	}

	// Set a small timer to change all the materials back to their previous states
	FTimerHandle THandle;
	FTimerDelegate TDelegate;

	TDelegate.BindLambda([this, MaxIndex]() {
		for (int i = 0; i < MaxIndex; i++)
		{
			DefaultMesh->SetMaterial(i, InitialMaterials[i]);
		}
	});

	GetWorldTimerManager().SetTimer(THandle, TDelegate, 0.05f, false);
}

void ADestructable::ClientAlignTextToFacePlayer_Implementation()
{
	// Find rotation to local player viewport
	FVector PlayerViewPoint;
	FRotator DiscardableRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPoint, DiscardableRotator);

	FRotator RotationToPlayerViewport = UKismetMathLibrary::FindLookAtRotation(HealthText->GetComponentLocation(), PlayerViewPoint);

	// Set transform to match what is intended
	HealthText->SetWorldTransform(FTransform(RotationToPlayerViewport, GetActorLocation() + HealthTextRelativePosition*GetActorScale3D(), FVector(1.f)));
}

void ADestructable::OnRep_Health()
{
	// Update health on text
	if (HealthText)
	{
		// Making it so that our health shows with one precision point
		double RoundedHealth = FMath::RoundHalfToZero(10.0 * Health) / 10.0;
		FString StringToSet = FString::SanitizeFloat(RoundedHealth);
		HealthText->SetText(StringToSet);
	}
}

void ADestructable::ServerTakeDamage_Implementation(float Damage)
{
	// Decreasing health and checking if this destructable should be destroyed
	Health -= Damage;
	OnRep_Health();

	if (Health <= 0)
	{
		OnDestroyed.Broadcast(this);
		SequenceDestroy();
	}

	// If wasn't destroyed, play hurt "animation"
	SequenceHurt();
}

bool ADestructable::ServerTakeDamage_Validate(float Damage)
{
	return true;
}

void ADestructable::ServerHealDamage_Implementation(float HealAmount)
{
	// Checking so that we don't go overboard with healing
	if (Health + HealAmount >= InitialHealth)
	{
		Health = InitialHealth;
	}
	else
	{
		Health += HealAmount;
	}

	OnRep_Health();
}

bool ADestructable::ServerHealDamage_Validate(float HealAmount)
{
	return true;
}

void ADestructable::ServerSetInitialHealth_Implementation(float HealthAmount)
{
	// Setting initial health and the current amount of health to what came in
	InitialHealth = HealthAmount;
	Health = InitialHealth;
	OnRep_Health();
}

bool ADestructable::ServerSetInitialHealth_Validate(float HealthAmount)
{
	return true;
}

void ADestructable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADestructable, Health);
	DOREPLIFETIME(ADestructable, InitialHealth);
}


