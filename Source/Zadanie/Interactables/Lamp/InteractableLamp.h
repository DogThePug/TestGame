// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Interactable.h"
#include "InteractableLamp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLightStateChangeSignature);

/**
 *  Lamp that player can interract with to turn on or off. Color of the lamp can also be changed.
 */
UCLASS()
class ZADANIE_API AInteractableLamp : public AInteractable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPointLightComponent* PointLight;

public:
	// Setting up default values and params
	AInteractableLamp();

	// Initiate starting state of the lamp
	virtual void BeginPlay() override;

	//  Handles beggining and end of hovering
	virtual void ClientBeginHover();
	virtual void ClientEndHover();

	// Change color of the light
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetLightColor(FLinearColor Color);

	// Add color to array and blend them together
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAddColorToBlend(FLinearColor Color);

	// Remove color from the array
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRemoveColorFromBlend(FLinearColor Color);

	// Indirectly interact with this lamp (Via button, for example)
	UFUNCTION(Server, Reliable, WithValidation)
	void IndirectInteract();

	// Set Intencity (Brightness) of the lamp by %
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetIntensityPercent(float Percent);

	// Get the state of light
	bool IsLightOn() const;

	// Delegate to notify that the state of this lamp was changed
	FOnLightStateChangeSignature OnStateChanged;
protected:
	// Implementing Interaction Functionality
	virtual void ServerInteractPostCheck() override;

	// Turning lamp on/off
	void SwitchLampState();

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	// Can player directly press on it to turn on/off
	UPROPERTY(EditAnywhere, Category = "FunctionalitySetup")
		bool bCanBeDirectlyInteractedWith = true;

private: // Private variables

	// Intensity that will be applied at full power
	UPROPERTY(EditAnywhere, Category = "FunctionalitySetup")
	float MaxLightIntensity = 700.f;

	// Current light intencity, regardless of the state
	UPROPERTY(Replicated, EditAnywhere, Category = "FunctionalitySetup")
	float CurrentLightIntensity = 700.f;

	// Current light intensity, including the state on/off
	UPROPERTY(ReplicatedUsing = OnRep_LightIntensity)
	float LightIntensity;


	// Current color of the light
	UPROPERTY(ReplicatedUsing = OnRep_LightColor)
	FLinearColor LightColor;

	// Is the light on or not
	bool bLightOn = true;

	// Array of color that are used to blend and change the current color of lamp light
	TArray<FLinearColor> ColorBlendArray;

private: // Private methods

	// Called on clients when server has changed intensity
	UFUNCTION()
	void OnRep_LightIntensity();

	// Called on clients when server has changed color
	UFUNCTION()
	void OnRep_LightColor();
};
