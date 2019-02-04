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
	// Constructor
	AInteractableLamp();

	// Change color of the light
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetLightColor(FLinearColor Color);

	// Add color to array and blend them together
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAddColorToBlend(FLinearColor Color);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRemoveColorFromBlend(FLinearColor Color);

	// Indirectly interact with this lamp (Via button, for example)
	UFUNCTION(Server, Reliable, WithValidation)
	void IndirectInteract();

	// Get the state of light
	bool IsLightOn() const;

	FOnLightStateChangeSignature OnStateChanged;
protected:
	// Implementing Interaction Functionality
	virtual void ServerInteractPostCheck() override;

	// Turning lamp on/off
	void SwitchLampState();

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

private: // Private variables

	// Replicated light intensity function
	UPROPERTY(ReplicatedUsing = OnRep_LightIntensity)
	float LightIntensity = 700.f;

	// Is the light on or not
	bool bLightOn = true;

	// Current color of the light
	UPROPERTY(ReplicatedUsing = OnRep_LightColor)
	FLinearColor LightColor;
	
	TArray<FLinearColor> ColorBlendArray;
private: // Private methods

	//Called on clients when server has changed intensity
	UFUNCTION()
	void OnRep_LightIntensity();

	// Called on clients when server has changed color
	UFUNCTION()
	void OnRep_LightColor();

	// Can player directly press on it to turn on/off
	UPROPERTY(EditAnywhere, Category = "FunctionalitySetup")
	bool bCanBeDirectlyInteractedWith = true;

};
