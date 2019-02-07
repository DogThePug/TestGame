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
	PrimaryActorTick.bCanEverTick = true;

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
	
	if (Role == ROLE_Authority)
	{
		Health = InitialHealth;
	}

	InitialMaterials = DefaultMesh->GetMaterials();

	OnRep_Health();

	if (bIsHealthVisible)
	{
		HealthText->SetVisibility(true);
		HealthTextRelativePosition = HealthText->GetRelativeTransform().GetLocation();
	}
	else
	{
		HealthText->DestroyComponent();
		SetActorTickEnabled(false);
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

// Called every frame
void ADestructable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthText)
	{
		ClientAlignTextToFacePlayer();
	}
}
void ADestructable::SequenceHurt_Implementation()
{
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
	Health -= Damage;
	OnRep_Health();

	if (Health <= 0)
	{
		OnDestroyed.Broadcast(this);
		SequenceDestroy();
	}

	SequenceHurt();
}

bool ADestructable::ServerTakeDamage_Validate(float Damage)
{
	return true;
}

void ADestructable::ServerHealDamage_Implementation(float HealAmount)
{
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


