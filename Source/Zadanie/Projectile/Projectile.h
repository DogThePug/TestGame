// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class ZADANIE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
	// Default mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ProjectileMesh;

	// Particles that play when projectile moves
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticlesMovement;

	// Particles that play when projectile hits something
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ParticlesOnHit;

	// ProjectileMovementComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* PMovementComp;
public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void NotifyHit
	(
		class UPrimitiveComponent * MyComp,
		AActor * Other,
		class UPrimitiveComponent * OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult & Hit
	) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Reacting to something that we have collided with
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReactToCollision(AActor* CollisionActor);

	// Spawning particles when we hit something
	UFUNCTION(NetMulticast, Reliable)
	void SpawnHitParticles();

	// Damage that this projectile does
	float Damage = 20.f;
	
	// Projectile Speed
	float Speed = 2300.f;

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
