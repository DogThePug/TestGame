// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Destructable.generated.h"

/**
* Base class for destructable actors. 
*/
UCLASS()
class ZADANIE_API ADestructable : public AActor
{
	GENERATED_BODY()


	// Static mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DefaultMesh;

	// On Death Particle effects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* OnDeathParticles;

	// Text that will show the remaining HP of this destructable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* HealthText;
public:	
	// Sets default values for this actor's properties
	ADestructable();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTakeDamage(float Damage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Current amount of health
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health = 100.f;
	
	// Initial Amount of Health
	UPROPERTY(EditAnywhere, Category = "Setup")
	float InitialHealth = 100.f;

	// Can we see health of given destructable or not
	UPROPERTY(EditAnywhere, Category = "Setup")
	bool bIsHealthVisible = true;

	// Destroying the object and spawning destruction particles
	UFUNCTION(NetMulticast, Reliable)
	void SequenceDestroy();

	UFUNCTION()
	void OnRep_Health();


	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
