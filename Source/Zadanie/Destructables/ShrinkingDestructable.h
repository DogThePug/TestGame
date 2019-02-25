// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Destructables/Destructable.h"
#include "ShrinkingDestructable.generated.h"

/**
 *  Destructable that shrinks (becomes smaller) with the amout of damage it takes. Also changes it's material when is below a certain health thresholds
 */
UCLASS()
class ZADANIE_API AShrinkingDestructable : public ADestructable
{
	GENERATED_BODY()

	// Material that is applied when this destructable has a lot of hp
	class UMaterial* HealthyMaterial;

	// Material that is applied when this destructable is below the first threshold
	class UMaterial* SlightlyHurtMaterial;

	// Material that is applied when this destructable is very low on healh
	class UMaterial* BadlyInjuredMaterial;

public:
	// Setting default values and properties
	AShrinkingDestructable();

	// Handles setting and recalculating of initial scale
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetInitialScale(FVector Scale);
protected:
	// Scale (3D) which is currently applied to the actor. 
	UPROPERTY(ReplicatedUsing = OnRep_InitialScale, EditAnywhere, Category = "Setup")
	FVector InitialScale = FVector(1.f);

	// Calculating scale after taking damage
	virtual void OnRep_Health() override;

	// When the initial scale changes we need to recalculate the scale that is applied based on missing HP
	UFUNCTION()
	void OnRep_InitialScale();

	// Calculating scale based on initial scale + how much health this destructable have lost 
	void SetScaleBasedOnMissingHealth();

	// Handles changing of material when health is below certain thresholds
	void SetMaterialBasedOnMissingHealth();

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
};
