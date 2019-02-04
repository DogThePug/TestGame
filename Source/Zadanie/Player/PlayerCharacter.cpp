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

#include "UnrealNetwork.h"

#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Enabling tick
	PrimaryActorTick.bCanEverTick = true;

	// Replication
	bReplicates = true;

	// Seting and configuring static mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Meshes/SM_MaterialSphere"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetStaticMesh(MeshAsset.Object);
	StaticMesh->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
}

void APlayerCharacter::PauseHoverTimer()
{
	GetWorldTimerManager().PauseTimer(HoverTimerHandle);
}

void APlayerCharacter::ContinueHoverTimer()
{
	GetWorldTimerManager().UnPauseTimer(HoverTimerHandle);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (Role == ROLE_Authority)
	{
		GetWorldTimerManager().SetTimer(HoverTimerHandle, this, &APlayerCharacter::ClientCheckHover, 1.f/30.f, true);
	}
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
}

UStaticMeshComponent * APlayerCharacter::GetStaticMesh() const
{
	return StaticMesh;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, HoverTimerHandle);
	DOREPLIFETIME(APlayerCharacter, CurrentTarget);
	DOREPLIFETIME(APlayerCharacter, PreviousTarget);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	// Check the range between us and currently interacted target. If we are too far away, end interaction
	if (bIsInteracting)
	{
		if (CurrentTarget)
		{
				if ((CurrentTarget->GetActorLocation() - GetActorLocation()).Size() > 200.f)
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
	bPressedJump = true;
}

void APlayerCharacter::JumpReleased()
{
	bPressedJump = false;
}

void APlayerCharacter::ClientBeginInteract_Implementation()
{
	ServerBeginInteract(this);
}

void APlayerCharacter::ClientEndInteract_Implementation()
{
	ServerEndInteract(this);
}


void APlayerCharacter::ClientCheckHover_Implementation()
{
	ServerBeginHover();

	if (CurrentTarget == PreviousTarget)
	{
		if (CurrentTarget)
		{
			CurrentTarget->ClientBeginHover();
		}
	}
	else
	{
		if (PreviousTarget)
		{
			PreviousTarget->ClientEndHover();
		}
	}
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


void APlayerCharacter::ServerBeginInteract_Implementation(APawn* Interactee)
{
	//// Return if we have no controller
	//if (!Controller) 
	//{
	//	return;
	//}

	//// Initiating local variables to perform line trace
	//FVector CamLoc;
	//FRotator CamRot;
	//Controller->GetPlayerViewPoint(CamLoc, CamRot);
	//const FVector StartTrace = CamLoc;
	//const FVector ShootDir = CamRot.Vector();
	//const FVector EndTrace = StartTrace + ShootDir * 100;

	//// Perform trace to retrieve hit info
	//FCollisionQueryParams TraceParams(FName(TEXT("InteractionTrace")), true, this);

	//TArray<FHitResult> HitResults;
	//GetWorld()->SweepMultiByChannel(HitResults, StartTrace, EndTrace, FQuat::Identity, ECollisionChannel::ECC_WorldDynamic, FCollisionShape::MakeSphere(20.f), TraceParams);

	//for (auto Hit : HitResults)
	//{
	//	AInteractable* PossibleInteractable = Cast<AInteractable>(Hit.Actor);
	//	if (PossibleInteractable)
	//	{
	//		PossibleInteractable->Interact();
	//		break;
	//	}
	//}

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
