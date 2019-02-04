// Made by Vladyslav Kulinych.

#include "RemoteDoor.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealNetwork.h"
#include "ConstructorHelpers.h"

// Sets default values
ARemoteDoor::ARemoteDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Turning replication on
	bReplicates = true;

	// Finding mesh for the door
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Meshes/SM_Shape_Cube"));

	// Seting root component to a new scene component
	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));

	// Initial door setup
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);
	if (MeshAsset.Object)
	{
		DoorMesh->SetStaticMesh(MeshAsset.Object);
	}
	DoorMesh->SetWorldScale3D(FVector(0.1f, 2.25f, 3.25f));
}

void ARemoteDoor::ServerInitiateStateChange_Implementation()
{
	if (Role == ROLE_Authority)
	{
		// Return if it is open and can't be closed
		if (bIsOpen && !bCanBeClosed) return;

		// If not in transition, set appropriate movement vector and start transition
		if (!bInTransition)
		{
			FVector DoorRightVector = DoorMesh->GetRelativeTransform().GetRotation().GetRightVector();
			CurrentDoorDirection = bIsOpen ? -DoorRightVector : DoorRightVector;
			bInTransition = true;
		}
	}
}


bool ARemoteDoor::ServerInitiateStateChange_Validate()
{
	return true;
}
// Called when the game starts or when spawned
void ARemoteDoor::BeginPlay()
{
	Super::BeginPlay();


}

void ARemoteDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARemoteDoor, bIsOpen);
	DOREPLIFETIME(ARemoteDoor, bInTransition);
	DOREPLIFETIME(ARemoteDoor, SlidingDistance);
	DOREPLIFETIME(ARemoteDoor, SlidingSpeed);
	DOREPLIFETIME(ARemoteDoor, CurrentDoorDirection);
	DOREPLIFETIME(ARemoteDoor, bCanBeClosed);
}

// Called every frame
void ARemoteDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInTransition)
	{
		float DistanceFromRoot = (DoorMesh->GetRelativeTransform().GetLocation()).Size();
		if (!bIsOpen)
		{
			if (SlidingDistance >= DistanceFromRoot)
			{
				DoorMesh->SetRelativeLocation(DoorMesh->GetRelativeTransform().GetLocation() + CurrentDoorDirection * SlidingSpeed);
			}
			else
			{
				DoorMesh->SetRelativeLocation(FVector(0.f, SlidingDistance, 0.f));
				bIsOpen = true;
				bInTransition = false;
			}
		}
		else
		{
			if (!FMath::IsNearlyEqual(DistanceFromRoot, 0.f, 0.1f))
			{
				DoorMesh->SetRelativeLocation(DoorMesh->GetRelativeTransform().GetLocation() + CurrentDoorDirection * SlidingSpeed);
			}
			else
			{
				DoorMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
				bIsOpen = false;
				bInTransition = false;
			}
		}
	}
}

