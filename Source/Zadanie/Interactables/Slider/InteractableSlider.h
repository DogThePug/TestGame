// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Interactable.h"
#include "InteractableSlider.generated.h"

/**
 *  Base class for sliders. Does nothing to the world on it's own, except for having a slider-like behavior.
 */
UCLASS()
class ZADANIE_API AInteractableSlider : public AInteractable
{
	GENERATED_BODY()

public:
	// Initial setup
	AInteractableSlider();

protected:
	// Slider Mesh that will be moving when we interact
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BlueprintProtected = "true"))
	class UStaticMeshComponent* SliderMesh;

	// Material that is applied to the root mesh when it is hovered over
	UPROPERTY(Category = "Materials", VisibleAnywhere, BlueprintReadOnly, meta = (BlueprintProtected = "true"))
	class UMaterial* HoveredMaterial;

	// Material that is applied to the root mesh when it is unhovered over
	UPROPERTY(Category = "Materials", VisibleAnywhere, BlueprintReadOnly, meta = (BlueprintProtected = "true"))
	class UMaterial* UnhoveredMaterial;

private:
	// Start of the slider
	UPROPERTY(Replicated)
	FVector Start;

	// End of the slider
	UPROPERTY(Replicated)
	FVector End;

	// Using this variable to set world location of a slider
	UPROPERTY(ReplicatedUsing = OnRep_SliderWorldLocation)
	FVector SliderWorldLocation;

	// Are we in the process of interacting
	UPROPERTY(Replicated)
	bool bInteracting = false;

	// Setting the location of a slider and it's value
	UFUNCTION()
	void OnRep_SliderWorldLocation();

	// Calculating the world position of a slider
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCalculateSliderPosition();

	// Calculating the relative position of a slider from the start and translates it to percent value
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCalculateSliderValue();

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
protected:
	// Current value of slider in %
	UPROPERTY(ReplicatedUsing = OnRep_SliderValue)
	float SliderValue = 0.f;

	// Calculate position where the slider should be, if we are interacting
	virtual void Tick(float DeltaTime) override;

	// Setting initial state and parameters
	virtual void BeginPlay() override;

	// Setting highlight on the slider
	virtual void ClientBeginHover() override;

	// Removing highlight from the slider
	virtual void ClientEndHover() override;

	// Starting interaction
	virtual void ServerInteractPostCheck();

	// Ending interation
	virtual void ServerEndInteractPostCheck();

	// Functionality to be implemented by children. What happens when the value of the slider in % changes
	UFUNCTION()
	virtual void OnRep_SliderValue();
};
