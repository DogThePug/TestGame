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

	// Replication
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
	// Ask a server to interract, for safety purposes
	ServerInteract();
}

void AInteractable::EndInteract()
{
	// Ask a server to end interaction
	ServerEndInteract();
}

void AInteractable::ClientBeginHover_Implementation()
{

}

void AInteractable::ClientEndHover_Implementation()
{
	
}

bool AInteractable::ServerInteract_Validate()
{
	return true;
}

void AInteractable::ServerInteract_Implementation()
{
	// Checking if we trully are the server
	if (Role == ROLE_Authority)
	{
		ServerInteractPostCheck();
	}
}


void AInteractable::ClientSetInteractee_Implementation(APawn * InteracteeToSet)
{
	Interactee = InteracteeToSet;
}

USphereComponent * const AInteractable::GetInteractionSphere() const
{
	return InteractionSphere;
}

void AInteractable::ServerInteractPostCheck()
{
	// ...
}

void AInteractable::ServerEndInteractPostCheck()
{
	// ...
}

void AInteractable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractable, Interactee);
}

void AInteractable::ServerEndInteract_Implementation()
{
	if (Role == ROLE_Authority)
	{
		ServerEndInteractPostCheck();
	}
}

bool AInteractable::ServerEndInteract_Validate()
{
	return true;
}



