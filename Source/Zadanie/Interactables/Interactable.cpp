// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "UnrealNetwork.h"
#include "ConstructorHelpers.h"
#include "../Player/PlayerCharacter.h"

// Set default values
AInteractable::AInteractable()
{
 	// Disabling tick
	PrimaryActorTick.bCanEverTick = false;

	// Network setup
	bReplicates = true;
	
	// Default mesh setup
	DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	RootComponent = DefaultMesh;
	DefaultMesh->SetGenerateOverlapEvents(true);
	DefaultMesh->SetCollisionProfileName("BlockAllDynamic");

	// Setting up the interaction sphere 
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(FName("InteractionSphere"));
	InteractionSphere->SetupAttachment(DefaultMesh);
	InteractionSphere->SetGenerateOverlapEvents(true);
	InteractionSphere->SetCollisionProfileName("OverlapAllDynamic");
	InteractionSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
}


void AInteractable::Interact()
{
	// Forwarding a call to server
	ServerInteract();
}

void AInteractable::EndInteract()
{
	// Ask a server to end interaction
	ServerEndInteract();
}


void AInteractable::ServerInteract_Implementation()
{
	if (Role == ROLE_Authority)
	{
		// Forwarding the call to overridable function
		ServerInteractPostCheck();
	}
}

bool AInteractable::ServerInteract_Validate()
{
	return true;
}

void AInteractable::ServerEndInteract_Implementation()
{
	if (Role == ROLE_Authority)
	{
		// Forwarding the call to overridable function
		ServerEndInteractPostCheck();
	}
}

bool AInteractable::ServerEndInteract_Validate()
{
	return true;
}

void AInteractable::ClientSetInteractee_Implementation(APawn * InteracteeToSet)
{
	// Setting pawn that currently interacts with this interactable
	Interactee = InteracteeToSet;
}

void AInteractable::ClientBeginHover_Implementation()
{
	// ...
}

void AInteractable::ClientEndHover_Implementation()
{
	// ...
}

void AInteractable::ServerInteractPostCheck()
{
	// ...
}

void AInteractable::ServerEndInteractPostCheck()
{
	// ...
}

USphereComponent * const AInteractable::GetInteractionSphere() const
{
	return InteractionSphere;
}

void AInteractable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractable, Interactee);
}


