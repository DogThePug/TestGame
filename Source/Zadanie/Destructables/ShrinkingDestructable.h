// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Destructables/Destructable.h"
#include "ShrinkingDestructable.generated.h"

/**
 * 
 */
UCLASS()
class ZADANIE_API AShrinkingDestructable : public ADestructable
{
	GENERATED_BODY()

	class UMaterial* HealthyMaterial;

	class UMaterial* SlightlyHurtMaterial;

	class UMaterial* BadlyInjuredMaterial;

public:
	AShrinkingDestructable();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetInitialScale(FVector Scale);
protected:
	UPROPERTY(ReplicatedUsing = OnRep_InitialScale, EditAnywhere, Category = "Setup")
	FVector InitialScale = FVector(1.f);

	virtual void OnRep_Health() override;

	UFUNCTION()
	void OnRep_InitialScale();

	void SetScaleBasedOnMissingHealth();

	void SetMaterialBasedOnMissingHealth();

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
};
