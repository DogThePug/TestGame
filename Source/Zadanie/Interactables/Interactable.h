// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

/**
*  Base class for all interactables. No functionality on it's own, only main framework for all interactables to use.
*/
UCLASS()
class ZADANIE_API AInteractable : public AActor
{
	GENERATED_BODY()

protected:
	// Default Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BlueprintProtected = "true"))
	class UStaticMeshComponent* DefaultMesh;

	// Sphere for interacting
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BlueprintProtected = "true"))
	class USphereComponent* InteractionSphere;

public:	
	// Sets default values for this actor's properties
	AInteractable();

	// Handles initiation of interaction and further server calls
	void Interact();
	
	// Function that ends interaction
	void EndInteract();

	// Handling when client begins to hover over this interactable
	UFUNCTION(Client, Unreliable)
	virtual void ClientBeginHover();

	// Handling ending hovering over this interactable
	UFUNCTION(Client, Unreliable)
	virtual void ClientEndHover();

	// Setting current interactee of this interactable
	UFUNCTION(Client, Unreliable)
	void ClientSetInteractee(class APawn* InteracteeToSet);

	// Getting interaction sphere of this interactable. 
	class USphereComponent* const GetInteractionSphere() const;
protected:
	// virtual function for children to implement interaction behavior
	virtual void ServerInteractPostCheck();

	// virtual function for children to implement interaction behavior
	virtual void ServerEndInteractPostCheck();

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	// Pawn that interacts with us
	UPROPERTY(Replicated)
	class APawn* Interactee;
private:
	// Function that executes on server when this object is interracted upon
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerInteract();

	// Function that executes on server when this object is interracted upon
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEndInteract();
};
