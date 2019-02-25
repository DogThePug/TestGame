// Made by Vladyslav Kulinych.

#include "InteractableSlider.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Components/SphereComponent.h"

#include "UnrealNetwork.h"

AInteractableSlider::AInteractableSlider()
{
	// Setting tick to true in order to figure out where the slider should be when it is interacted with
	PrimaryActorTick.bCanEverTick = true;

	// Set default mesh 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("DefaultMesh'/Game/Meshes/SM_SliderBase.SM_SliderBase'"));
	if (MeshAsset.Object)
	{
		DefaultMesh->SetStaticMesh(MeshAsset.Object);
	}

	// Setup slider mesh
	SliderMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Slider"));
	SliderMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SliderMeshAsset(TEXT("DefaultMesh'/Game/Meshes/SM_Slider1.SM_Slider1'"));
	
	if (SliderMeshAsset.Object)
	{
		SliderMesh->SetStaticMesh(SliderMeshAsset.Object);
	}

	SliderMesh->SetRelativeLocation(FVector(0.f, 10.f, 0.f));

	// Attaching interaction sphere to the movable part of the slider
	InteractionSphere->SetupAttachment(SliderMesh);

	// Setup start and end points of the slider
	Start = FVector(0.f, 10.f, 0.f);
	End = FVector(200.f, 10.f, 0.f);
}

void AInteractableSlider::BeginPlay()
{
	Super::BeginPlay();

	// Set the end of a slider relative to it's scale and rotation so that we can rotate and scale the slider however we like
	if (Role == ROLE_Authority)
	{
		Start *= GetActorScale3D();
		End *= GetActorScale3D();

		Start = GetActorRotation().RotateVector(Start);
		End = GetActorRotation().RotateVector(End);

		SliderValue = 0.f;
		OnRep_SliderValue();
	}
}

void AInteractableSlider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Calculate the position of slider
	if (Role == ROLE_Authority)
	{
		if (bInteracting)
		{
			if (Interactee)
			{
				ServerCalculateSliderPosition();
			}
		}
	}
	else
	{
		// turn tick off if we are not the server
		SetActorTickEnabled(false);
	}
}

void AInteractableSlider::ServerCalculateSliderPosition_Implementation()
{
	// Calculating where the slider should be based on the interactee point of view
	if (Interactee)
	{
		// Initiating local variables
		FVector CamLoc;
		FRotator CamRot;

		Interactee->GetController()->GetPlayerViewPoint(CamLoc, CamRot);
		const FVector StartTrace = CamLoc;
		const FVector ShootDir = CamRot.Vector();
		const FVector EndTrace = StartTrace + ShootDir * 200;

		// Find the intersection point between the direction of where the interactee is looking and the plane in which slider is on
		FVector Intersection = FMath::LinePlaneIntersection(
			StartTrace,
			EndTrace,
			GetActorLocation(),
			GetActorRotation().RotateVector(FVector(0.f, 1.f, 0.f))
		);

		// Find the point on the line of a slider
		FVector PointOnSliderLine = FMath::ClosestPointOnLine(GetActorLocation() + Start, GetActorLocation() + End, Intersection);

		// Seting the world location variable to the point
		SliderWorldLocation = PointOnSliderLine;

		// Force calling the change of position for server
		OnRep_SliderWorldLocation();
	}
}

void AInteractableSlider::ServerCalculateSliderValue_Implementation()
{
	// Calculating the value of slider based on it's relative position between the start and end vectors in local spaceS
	if (Role == ROLE_Authority)
	{
		// Finding the start and end world locations
		FVector SliderWorldStart = GetActorLocation() + Start;
		FVector SliderWorldEnd = GetActorLocation() + End;

		// Finding distance from start to slider and from start to end
		float DistanceFromStartToSlider = (SliderWorldStart - SliderMesh->GetComponentLocation()).Size();
		float DistanceFromStartToEnd = (SliderWorldStart - SliderWorldEnd).Size();

		// Comparing them to get the percent
		float PercentValue = DistanceFromStartToSlider / DistanceFromStartToEnd;

		// Changing slider value to the calculated one
		SliderValue = PercentValue;
		OnRep_SliderValue();
	}
}

bool AInteractableSlider::ServerCalculateSliderPosition_Validate()
{
	return true;
}

void AInteractableSlider::OnRep_SliderWorldLocation()
{
	// Set slider location
	SliderMesh->SetWorldLocation(SliderWorldLocation);

	if (Role == ROLE_Authority)
	{
		// Calculate slider value
		ServerCalculateSliderValue();
	}
}

void AInteractableSlider::ClientBeginHover()
{
	// Setting the slider vector parameter to signify that we are hovering over it
	SliderMesh->SetVectorParameterValueOnMaterials(FName("Color"), FVector(0.98f, 0.346f, 0.5f));
}

void AInteractableSlider::ClientEndHover()
{	
	// Setting the slider vector parameter to signify that we are no longer hovering over it
	SliderMesh->SetVectorParameterValueOnMaterials(FName("Color"), FVector(0.98f, 0.346f, 0.f));
}

void AInteractableSlider::ServerInteractPostCheck()
{
	// Starting interaction
	bInteracting = true;
}

void AInteractableSlider::ServerEndInteractPostCheck()
{
	// Ending interaction
	bInteracting = false;
}




bool AInteractableSlider::ServerCalculateSliderValue_Validate()
{
	return true;
}

void AInteractableSlider::OnRep_SliderValue()
{
	//...
}
void AInteractableSlider::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractableSlider, SliderValue);
	DOREPLIFETIME(AInteractableSlider, Start);
	DOREPLIFETIME(AInteractableSlider, End);
	DOREPLIFETIME(AInteractableSlider, bInteracting); 
	DOREPLIFETIME(AInteractableSlider, SliderWorldLocation);
}
