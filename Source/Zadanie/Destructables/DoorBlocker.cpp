// Made by Vladyslav Kulinych.

#include "DoorBlocker.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "ConstructorHelpers.h"
#include "../RemoteDoor/BlockedDoor.h"
#include "Particles/ParticleSystem.h"

#include "UnrealNetwork.h"


ADoorBlocker::ADoorBlocker()
{
	// Setting initial health
	InitialHealth = 30.f;

	// Set health to invisible
	bIsHealthVisible = false;

	// We need this one to be stationary
	DefaultMesh->SetEnableGravity(false);
	DefaultMesh->SetSimulatePhysics(false);

	// Setting up new default mesh and material for it
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Meshes/SM_WallOfRock_00"));

	if (MeshAsset.Object)
	{
		DefaultMesh->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Materials/M_JaggedRock_01"));

	if (MaterialAsset.Object)
	{
		DefaultMesh->SetMaterial(0, MaterialAsset.Object);
	}

	// Finding default particles asset for on hit particles
	static ConstructorHelpers::FObjectFinder<UParticleSystem> OnDeathParticlesAsset(TEXT("ParticleSystem'/Game/Particles/P_SteamBurst.P_SteamBurst'"));

	if (OnDeathParticlesAsset.Object)
	{
		OnDeathParticles = OnDeathParticlesAsset.Object;
	}
}


void ADoorBlocker::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		if (BlockedDoor)
		{
			BlockedDoor->ServerConnectDoorBlocker(this);
		}
	}
}
