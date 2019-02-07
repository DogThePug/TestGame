// Made by Vladyslav Kulinych.

#include "Projectile.h"
#include "UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Destructables/Destructable.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	// Setting up projectile mesh
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Projectile"));
	ProjectileMesh->SetCollisionProfileName("BlockAllDynamic");
	ProjectileMesh->SetGenerateOverlapEvents(true);
	ProjectileMesh->SetSimulatePhysics(true);
	ProjectileMesh->SetEnableGravity(false);
	ProjectileMesh->SetNotifyRigidBodyCollision(true);

	ProjectileMesh->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));

	// Finding mesh asset for projectile and setting it
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Meshes/SM_TapLob_Projectile_Ice_Bomblet"));

	if (MeshAsset.Object)
	{
		ProjectileMesh->SetStaticMesh(MeshAsset.Object);
	}

	// Finding and setting material for projectile
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialAsset(TEXT("/Game/Materials/M_BottomTrim_Inst_Black"));
	UMaterial* ProjectileMaterial = (UMaterial*)MaterialAsset.Object;

	if (ProjectileMaterial)
	{
		ProjectileMesh->SetMaterial(0, ProjectileMaterial);
	}

	RootComponent = ProjectileMesh;


	// Setting up movement particles

	ParticlesMovement = CreateDefaultSubobject<UParticleSystemComponent>(FName("MovementParticles"));
	ParticlesMovement->bAutoActivate = true;
	ParticlesMovement->SetupAttachment(RootComponent);

	// Finding particle asset for movement particles
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticlesAsset(TEXT("/Game/Particles/P_Water_Projectile"));

	if (ParticlesAsset.Object)
	{
		ParticlesMovement->SetTemplate(ParticlesAsset.Object);
	}

	// Finding particles asset for on hit particles
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticlesAsset(TEXT("/Game/Particles/P_spark_burst_2"));

	if (HitParticlesAsset.Object)
	{
		ParticlesOnHit = HitParticlesAsset.Object;
	}

	// Seting up projectile movement
	PMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	PMovementComp->SetUpdatedComponent(RootComponent);
	PMovementComp->SetIsReplicated(true);
	PMovementComp->InitialSpeed = Speed;
	PMovementComp->ProjectileGravityScale = 0.f;
	PMovementComp->bSimulationEnabled = true;
	PMovementComp->bShouldBounce = false;
}

void AProjectile::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Role == ROLE_Authority)
	{
		ServerReactToCollision(Other);
	}
}



// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();


}

void AProjectile::SpawnHitParticles_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticlesOnHit, GetActorLocation(), GetActorRotation());
}

void AProjectile::ServerReactToCollision_Implementation(AActor * CollisionActor)
{
	// Play particles on hit
	SpawnHitParticles();

	ADestructable* PossibleDestructable = Cast<ADestructable>(CollisionActor);
	if(PossibleDestructable)
	{
		PossibleDestructable->ServerTakeDamage(Damage);
	}

	// Destroy this actor
	Destroy();
}

bool AProjectile::ServerReactToCollision_Validate(AActor * CollisionActor)
{
	return true;
}

void AProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

