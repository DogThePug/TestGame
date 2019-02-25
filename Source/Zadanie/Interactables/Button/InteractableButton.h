// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Interactable.h"
#include "InteractableButton.generated.h"

/**
 * Base class for buttons. Buttons are used to interact and change properties with other actors at a distance
 */
UCLASS()
class ZADANIE_API AInteractableButton : public AInteractable
{
	GENERATED_BODY()

protected:
	AInteractableButton();

	// Send invitations to tie to all the tied buttons on begin play
	virtual void BeginPlay() override;

	// Material that is applied to the mesh when it is toggled/activated
	UPROPERTY(Category = "Materials", VisibleAnywhere, BlueprintReadOnly, meta = (BlueprintProtected = "true"))
	class UMaterial* ToggledMaterial;

	// Material that is applied to the mesh when it is untoggled/not activated
	UPROPERTY(Category = "Materials", VisibleAnywhere, BlueprintReadOnly, meta = (BlueprintProtected = "true"))
	class UMaterial* UntoggledMaterial;

	// Material that is applied to the root mesh when it is hovered over
	UPROPERTY(Category = "Materials", VisibleAnywhere, BlueprintReadOnly, meta = (BlueprintProtected = "true"))
	class UMaterial* HoveredMaterial;

	// Material that is applied to the root mesh when it is not hovered over
	UPROPERTY(Category = "Materials", VisibleAnywhere, BlueprintReadOnly, meta = (BlueprintProtected = "true"))
	class UMaterial* UnhoveredMaterial;

	// Button that will be a main visual guidence to determine if this interactable is toggled or not
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BlueprintProtected = "true"))
	class UStaticMeshComponent* Button;

	// Current state of a button
	UPROPERTY(ReplicatedUsing = OnRep_IsToggled, Category = "FunctionalitySetup", EditAnywhere, meta = (BlueprintProtected = "true"))
	bool bIsToggled = false;

	// Determines if we can untoggle it once we toggled it
	UPROPERTY(Replicated, Category = "FunctionalitySetup", EditAnywhere, meta = (BlueprintProtected = "true"))
	bool bCanBeToggledOff = true;

	// Can we check this button, or does it uncheck by itself
	UPROPERTY(Replicated, EditAnywhere, Category = "FunctionalitySetup")
	bool bIsCheckable = true;

	// Array of buttons to deactivate when we activate this one. Sends invitations to them, ensuring that when they are interacted with, this button gets untoggled as well
	UPROPERTY(Replicated, Category = "FunctionalitySetup", EditAnywhere, meta = (BlueprintProtected = "true"))
	TArray<AInteractableButton*> TiedButtonsArray;

	// Virtual function to be overriden by children to implement what this button does when toggled
	virtual void ButtonEffectOnToggle();

	// Virtual function to be overriden by children to implement what this button does when untoggled
	virtual void ButtonEffectOnUntoggle();

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	// Handles hovering over this interactable
	virtual void ClientBeginHover() override;

	// Handles unhovering over this interactable
	virtual void ClientEndHover() override;

public:
	// Implementing Interaction Functionality
	virtual void ServerInteractPostCheck() override;

	// Other buttons might ask about adding them into the array of tied to buttons
	void AddButtonToArray(AInteractableButton* Button);

	// Changing the toggle state of this button
	virtual void SetToggledState(bool bVal);

	// Sets proper transform and material on the client side when IsToggled changed
	UFUNCTION()
	void OnRep_IsToggled();
};
