// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Destructable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestructionSignature, ADestructable*, DestroyedSelf);

/**
* Base class for destructable actors. 
*/
UCLASS()
class ZADANIE_API ADestructable : public AActor
{
	GENERATED_BODY()

protected:
	// Static mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DefaultMesh;

	// On Death Particle effects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* OnDeathParticles;

	// Text that will show the remaining HP of this destructable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* HealthText;

	// Material that is set when destructable took damage
	class UMaterial* HurtMaterial;
public:	
	// Sets default values for this actor's properties
	ADestructable();

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerTakeDamage(float Damage);

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerHealDamage(float HealAmount);

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerSetInitialHealth(float HealthAmount);

	FOnDestructionSignature OnDestroyed;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Current amount of health
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health = 100.f;
	
	// Initial Amount of Health
	UPROPERTY(Replicated, EditAnywhere, Category = "Setup")
	float InitialHealth = 100.f;

	FVector HealthTextRelativePosition;
	// Can we see health of given destructable or not
	UPROPERTY(EditAnywhere, Category = "Setup")
	bool bIsHealthVisible = true;

	// Destroying the object and spawning destruction particles
	UFUNCTION(NetMulticast, Reliable)
	void SequenceDestroy();

	// Changes the material of the object to signify that it is being hurt for a short time, then returns it back to it's previous material
	UFUNCTION(NetMulticast, Reliable)
	void SequenceHurt();

	// Finds the client player and faces the text towards him
	UFUNCTION(Client, Reliable)
	void ClientAlignTextToFacePlayer();

	// Updates the amount of health shown
	UFUNCTION()
	virtual void OnRep_Health();

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	// Initial materials on the destructable actor. Used to flicker the damage material when this actor is damaged
	TArray<UMaterialInterface*> InitialMaterials;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
