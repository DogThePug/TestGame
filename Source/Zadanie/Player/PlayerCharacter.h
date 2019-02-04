// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ZADANIE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	// Static mesh for this character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Are we in the process of interacting with something
	UPROPERTY(Replicated)
	bool bIsInteracting = false;

	// Pausing hover timer
	void PauseHoverTimer();

	// Unpausing hover timer
	void ContinueHoverTimer();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Timer that sends sweeps to find interactables
	UPROPERTY(Replicated)
	struct FTimerHandle HoverTimerHandle;

	// Current target that we are focused on
	UPROPERTY(Replicated)
	class AInteractable* CurrentTarget;

	// Target from a previous frame
	UPROPERTY(Replicated)
	class AInteractable* PreviousTarget;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Return inherited mesh of this character
	UFUNCTION()
	UStaticMeshComponent* GetStaticMesh() const;

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	// Break the interaction if we are too far away from the thing we are interacting with
	virtual void Tick(float DeltaTime) override;
private: 
	/// Movement Behavior Setup
	// Handles moving forward/backward
	UFUNCTION()
	void MoveForward(float Val);
	
	// Handles strafing
	UFUNCTION()
	void MoveRight(float Val);

	// Handles jumping
	UFUNCTION()
	void JumpPressed();

	UFUNCTION()
	void JumpReleased();

	/// Interaction Begavior Setup
	// Starting the interaction on the client side in order to send self as an interactor
	UFUNCTION(Client, Reliable)
	void ClientBeginInteract();

	// Stoping the interaction on the client side
	UFUNCTION(Client, Reliable)
	void ClientEndInteract();

	// Engages interacting behavior from server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBeginInteract(APawn* Interactee);

	// Engages interacting behavior from server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEndInteract(APawn* Interactee);

	// Handles hovering
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBeginHover();

	// Tells an interactable that we had/have a focus on to change it's appearence for the client
	UFUNCTION(Client, Reliable)
	void ClientCheckHover();

};
