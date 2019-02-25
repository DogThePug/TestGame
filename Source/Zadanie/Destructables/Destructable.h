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
	// Root mesh of this actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DefaultMesh;

	// On Death Particle effects. Are spawned when this actor is destroyed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* OnDeathParticles;

	// Text that will show the remaining HP of this destructable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* HealthText;

	// Material that is set when destructable takes damage
	class UMaterial* HurtMaterial;
public:	
	// Sets default values for this actor's properties
	ADestructable();

	// Handles taking damage
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerTakeDamage(float Damage);

	// Handles healing damage
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerHealDamage(float HealAmount);

	// Handles setting of initial health and current health 
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerSetInitialHealth(float HealthAmount);

	// Delegate that is broadcasted once this destructable dies
	FOnDestructionSignature OnDestroyed;
protected:
	// Initiating behavior and properties during the rest of the game
	virtual void BeginPlay() override;

	// Current amount of health
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health = 100.f;
	
	// Initial Amount of Health
	UPROPERTY(Replicated, EditAnywhere, Category = "Setup")
	float InitialHealth = 100.f;

	// Can we see health of given destructable or not
	UPROPERTY(EditAnywhere, Category = "Setup")
	bool bIsHealthVisible = true;

	// Relative position of text that is needed to properely update the position of the text
	FVector HealthTextRelativePosition;

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
