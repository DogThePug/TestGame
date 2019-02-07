// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Destructables/Destructable.h"
#include "DestructableHealer.generated.h"

/**
 * 
 */
UCLASS()
class ZADANIE_API ADestructableHealer : public ADestructable
{
	GENERATED_BODY()
	
	// Particles that play when this actor shares damage with another
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ParticlesOnHeal;

	// Sharer sphere.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* HealSphere;
public:
	ADestructableHealer();

	// Sets the radius at which the damage is shared
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetHealRadius(float Radius);

	// Sets the % of damage that is shared with enemies close by
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetHealAmount(float Amount);

	// Set the frequency of heals
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetHealTime(float Time);
protected:
	// Start a timer to heal every N seconds
	virtual void BeginPlay();

	FTimerHandle HealTimerHandle;

	// Function that finds destructables in vicinity and heals them
	void HealDamage();

	// Spawns particles on all connected clients
	UFUNCTION(NetMulticast, Reliable)
	void SpawnEmmiterTo(ADestructable* Destructable);

	// Radius in which this destructable shares portion of it's damage it's damage
	UPROPERTY(EditAnywhere, Category = "Setup")
		float HealRadius = 350.f;

	// % of damage that is shared among the destructables in zone
	UPROPERTY(EditAnywhere, Category = "Setup", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float HealAmount = 2.f;

	// In how many seconds will healing effect take place (0.01666666 = Every frame)
	UPROPERTY(EditAnywhere, Category = "Setup", meta = (ClampMin = "0.01666666666", ClampMax = "10.0"))
	float HealTime = 0.2f;

};
