// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "UnrealNetwork.h"
#include "ConstructorHelpers.h"

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


void AInteractable::ServerInteractPostCheck()
{
	// ...
}

void AInteractable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ...
}


