// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Destructables/Destructable.h"
#include "DamageSharer.generated.h"

/**
 *  Destructable that shares it's damage taken with other destructables in vicinity
 */
UCLASS()
class ZADANIE_API ADamageSharer : public ADestructable
{
	GENERATED_BODY()

	// Particles that play when this actor shares damage with another
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ParticlesOnShare;

	// Sphere in which overlapping destructables will be forced to take damage whenever this destructable takes some
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ShareSphere;
public:
	// Setting default values and components
	ADamageSharer();

	// Sets the radius at which the damage is shared
	UFUNCTION(Server,Reliable,WithValidation)
	void ServerSetShareRadius(float Radius);

	// Sets the % of damage that is shared with enemies close by
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetShareAmount(float Amount);
protected:
	// Before this destructable will take damage it will share some of it with destructables in vicinity
	virtual void ServerTakeDamage(float Damage) override;

	// Function that finds destructables in vicinity and shares damage with them
	void ShareDamage(float& Damage);

	// Spawns particles on all connected clients
	UFUNCTION(NetMulticast, Reliable)
	void SpawnEmmiterTo(ADestructable* Destructable);

	// Radius in which this destructable shares portion of it's damage it's damage
	UPROPERTY(EditAnywhere, Category = "Setup")
	float ShareRadius = 350.f;

	// % of damage that is shared among the destructables in zone
	UPROPERTY(EditAnywhere, Category = "Setup", meta = ( ClampMin = "0.0", ClampMax = "1.0" ))
	float ShareAmount = 0.5f;
};
