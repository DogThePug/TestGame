// Made by Vladyslav Kulinych.

#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "../Interactables/Interactable.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Disabling tick
	PrimaryActorTick.bCanEverTick = false;

	// Replication
	bReplicates = true;

	// Seting and configuring static mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Meshes/SM_MaterialSphere"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetStaticMesh(MeshAsset.Object);
	StaticMesh->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	InputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::TryInterract);
}

UStaticMeshComponent * APlayerCharacter::GetStaticMesh() const
{
	return StaticMesh;
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

void APlayerCharacter::TryInterract_Implementation()
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

	TArray<FHitResult> HitResults;
	GetWorld()->SweepMultiByChannel(HitResults, StartTrace, EndTrace, FQuat::Identity, ECollisionChannel::ECC_WorldDynamic, FCollisionShape::MakeSphere(20.f), TraceParams);

	for (auto Hit : HitResults)
	{
		AInteractable* PossibleInteractable = Cast<AInteractable>(Hit.Actor);
		if (PossibleInteractable)
		{
			PossibleInteractable->Interact();
			break;
		}
	}
}

bool APlayerCharacter::TryInterract_Validate()
{
	return true;
}


