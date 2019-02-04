// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

/**
* Base class for all interactables
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

	// Function that initialises interaction
	void Interact();

	UFUNCTION(Client, Unreliable)
	virtual void ClientBeginHover();

	UFUNCTION(Client, Unreliable)
	virtual void ClientEndHover();
protected:
	// virtual function for children to implement interact behavior
	virtual void ServerInteractPostCheck();

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
private:
	// Function that executes on server when this object is interracted upon
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerInteract();

	
	
};
