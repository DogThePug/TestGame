// Made by Vladyslav Kulinych.

#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "../Interactables/Interactable.h"
#include "Components/SphereComponent.h"
#include "../Projectile/Projectile.h"

#include "UnrealNetwork.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Enabling tick
	PrimaryActorTick.bCanEverTick = true;

	// Replication
	bReplicates = true;

	// Seting and configuring default mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Meshes/SM_MaterialSphere"));

	DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	DefaultMesh->SetupAttachment(RootComponent);
	DefaultMesh->SetStaticMesh(MeshAsset.Object);
	DefaultMesh->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Binding movement inputs
	PlayerInputComponent->BindAxis("ForwardMovement", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("RightMovement", this, &APlayerCharacter::MoveRight);

	// Binding "Looking" input
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);

	// Binding jumping
	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::JumpPressed);
	InputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::JumpReleased);

	//Binding interaction
	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::ClientBeginInteract);
	InputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::ClientEndInteract);

	// Binding shooting
	InputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerCharacter::ServerShoot);

}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Starting timer for hover behavior
	if (Role == ROLE_Authority)
	{
		GetWorldTimerManager().SetTimer(HoverTimerHandle, this, &APlayerCharacter::ClientCheckHover, 1.f/30.f, true);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	// Check the range between us and currently interacted target. If we are too far away, end interaction
	if (bIsInteracting)
	{
		if (CurrentTarget)
		{
			if ((CurrentTarget->GetInteractionSphere()->GetComponentLocation() - GetActorLocation()).Size() > 200.f)
			{
				ClientEndInteract();
			}
		}
	}
}


void APlayerCharacter::MoveForward(float Val)
{
	if ((Controller != NULL) && (Val != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();

		// Limit pitch when walking or falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}

		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Val);
	}
}

void APlayerCharacter::MoveRight(float Val)
{
	if ((Controller != NULL) && (Val != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

		// add movement in that direction
		AddMovementInput(Direction, Val);
	}
}

void APlayerCharacter::JumpPressed()
{
	// Handling jumping
	bPressedJump = true;
}

void APlayerCharacter::JumpReleased()
{
	bPressedJump = false;
}


void APlayerCharacter::ClientCheckHover_Implementation()
{
	// Checking through server if we have any targets in front of us
	ServerBeginHover();

	// If the current target is the one we had before, tell it that we are still hovering over it
	if (CurrentTarget == PreviousTarget)
	{
		if (CurrentTarget)
		{
			CurrentTarget->ClientBeginHover();
		}
	}
	else
	{
		// If our target was not the previous one, tell the previous one that we hover over it no longer
		if (PreviousTarget)
		{
			PreviousTarget->ClientEndHover();
		}
	}

	// Set the previous target to the current one to check it on the next cycle
	PreviousTarget = CurrentTarget;
}

void APlayerCharacter::ServerBeginHover_Implementation()
{
	// Return if we have no controller
	if (!Controller)
	{
		return;
	}

	// Initiating local variables to perform line trace
	FVector CamLoc;
	FRotator CamRot;
	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector StartTrace = CamLoc;
	const FVector ShootDir = CamRot.Vector();
	const FVector EndTrace = StartTrace + ShootDir * 100;

	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(FName(TEXT("InteractionTrace")), true, this);

	FHitResult HitResult;
	GetWorld()->SweepSingleByChannel(HitResult, StartTrace, EndTrace, FQuat::Identity, ECollisionChannel::ECC_WorldDynamic, FCollisionShape::MakeSphere(20.f), TraceParams);

	// If we found an interactable, set it as a current target
	AInteractable* PossibleInteractable = Cast<AInteractable>(HitResult.Actor);
	if (PossibleInteractable)
	{
		CurrentTarget = PossibleInteractable;
	}
	else
	{
		CurrentTarget = nullptr;
	}
}

bool APlayerCharacter::ServerBeginHover_Validate()
{
	return true;
}

void APlayerCharacter::ClientBeginInteract_Implementation()
{
	// Telling the server that local player wants to interact
	ServerBeginInteract(this);
}

void APlayerCharacter::ClientEndInteract_Implementation()
{
	// Telling the server that local player wants to end interacting
	ServerEndInteract(this);
}

void APlayerCharacter::ServerBeginInteract_Implementation(APawn* Interactee)
{
	/*
	* Telling our current target to start start interaction, sending the interactee as a reference for further actions
	* and setting both interactee and interactable to the state of interaction
	*/
	if (CurrentTarget)
	{
		APlayerCharacter* InteracteeCharacter = Cast<APlayerCharacter>(Interactee);
		
		if (InteracteeCharacter)
		{
			CurrentTarget->ClientSetInteractee(Interactee);
			InteracteeCharacter->bIsInteracting = true;
			InteracteeCharacter->PauseHoverTimer();
			CurrentTarget->Interact();
		}
	}
}

bool APlayerCharacter::ServerBeginInteract_Validate(APawn* Interactee)
{
	return true;
}

void APlayerCharacter::ServerEndInteract_Implementation(APawn* Interactee)
{
	// Telling our current target that we no longer interact with it, reseting both interactee and interactable to the initial state
	if (CurrentTarget)
	{
		APlayerCharacter* InteracteeCharacter = Cast<APlayerCharacter>(Interactee);
		if (InteracteeCharacter)
		{
			CurrentTarget->EndInteract();
			CurrentTarget->ClientSetInteractee(nullptr);
			InteracteeCharacter->bIsInteracting = false;
			InteracteeCharacter->ContinueHoverTimer();
		}
	}

}

bool APlayerCharacter::ServerEndInteract_Validate(APawn* Interactee)
{
	return true;
}

void APlayerCharacter::PauseHoverTimer()
{
	// Pausing hover timer when we interact in order to not hover anything else when we are in the process of interacting
	GetWorldTimerManager().PauseTimer(HoverTimerHandle);
}

void APlayerCharacter::ContinueHoverTimer()
{
	// Continuing hover timer when we no longer interact
	GetWorldTimerManager().UnPauseTimer(HoverTimerHandle);
}

void APlayerCharacter::ServerShoot_Implementation()
{
	// Finding forward rotation through local controller and spawning a projectile
	FVector CamLoc;
	FRotator CamRot;
	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector StartTrace = CamLoc;
	const FVector ShootDir = CamRot.Vector();
	
	GetWorld()->SpawnActor<AProjectile>(GetActorLocation() + ShootDir * 80.f + FVector(0.f,0.f,52.f), CamRot);
}

bool APlayerCharacter::ServerShoot_Validate()
{
	return true;
}

UStaticMeshComponent * APlayerCharacter::GetStaticMesh() const
{
	return DefaultMesh;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, HoverTimerHandle);
	DOREPLIFETIME(APlayerCharacter, CurrentTarget);
	DOREPLIFETIME(APlayerCharacter, PreviousTarget);
}