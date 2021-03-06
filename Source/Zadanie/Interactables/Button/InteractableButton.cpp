// Made by Vladyslav Kulinych.

#include "InteractableButton.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealNetwork.h"
#include "Materials/Material.h"
#include "TimerManager.h"


AInteractableButton::AInteractableButton() 
{
	// Finding materials
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> UntoggledMaterialAsset(TEXT("MaterialInstanceConstant'/Game/Materials/M_Metal_BrushedSteel_Inst.M_Metal_BrushedSteel_Inst'"));
	UntoggledMaterial = (UMaterial*)(UntoggledMaterialAsset.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> ToggledMaterialAsset(TEXT("/Game/Materials/M_Metal_Gold_Inst"));
	ToggledMaterial = (UMaterial*)(ToggledMaterialAsset.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> HoveredMaterialAsset(TEXT("/Game/Materials/M_MaterialSphere_2"));
	HoveredMaterial = HoveredMaterialAsset.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> UnhoveredMaterialAsset(TEXT("/Game/Materials/M_Metal_BrushedSteel_Inst"));
	UnhoveredMaterial = (UMaterial*)UnhoveredMaterialAsset.Object;

	// Set default mesh 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Meshes/SM_DeathPlane_01"));
	DefaultMesh->SetStaticMesh(MeshAsset.Object);

	if (UnhoveredMaterial)
	{
		DefaultMesh->SetMaterial(0, UnhoveredMaterial);
	}

	// Set button mesh
	Button = CreateDefaultSubobject<UStaticMeshComponent>(FName("Button"));
	Button->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ButtonAsset(TEXT("/Game/Meshes/SM_MaterialSphere"));
	Button->SetStaticMesh(ButtonAsset.Object);
	Button->SetRelativeLocation(FVector(0.f, 0.f, 2.f));
	Button->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.2f));

	if (UntoggledMaterial)
	{
		Button->SetMaterial(0, UntoggledMaterial);
	}
}

void AInteractableButton::BeginPlay()
{
	Super::BeginPlay();

	// Seting our initial state
	SetToggledState(bIsToggled);

	// Ask all the buttons we got tied to to add us into their ties
	for (auto TiedButton : TiedButtonsArray)
	{
		if (TiedButton)
		{
			TiedButton->AddButtonToArray(this);
		}
	}
}


void AInteractableButton::AddButtonToArray(AInteractableButton * Button)
{
	// Check if we already have that button in our array. If we do, return
	for (auto TiedButton : TiedButtonsArray)
	{
		if (TiedButton)
		{
			if (TiedButton->GetName() == Button->GetName())
				return;
		}
	}

	// If we didn't find a button that tries to tie with us, add it to array
	TiedButtonsArray.Add(Button);
}

void AInteractableButton::ServerInteractPostCheck()
{
	// Handling togling and effects
	if (!bIsToggled)
	{
		// Untoggle all tied to buttons
		for (auto TiedButton : TiedButtonsArray)
		{
			if (TiedButton)
			{
				TiedButton->SetToggledState(false);
			}
		}

		// Toggle this button
		SetToggledState(true);
		ButtonEffectOnToggle();

		// If this button is not checkable, set a timer to automaticly untoggle it
		if (!bIsCheckable)
		{
			FTimerHandle TimerHandle;

			FTimerDelegate TDelegate;
			TDelegate.BindLambda([this]() {
				SetToggledState(false);
			});

			GetWorldTimerManager().SetTimer(TimerHandle, TDelegate, 0.1f, false);
		}
	}
	else
	{
		// Untoggle if we can
		if (bIsCheckable)
		{
			if (bCanBeToggledOff)
			{
				SetToggledState(false);
				ButtonEffectOnUntoggle();
			}
		}
	}
}

void AInteractableButton::SetToggledState(bool State)
{
	// Setting toggled state to another 
	bIsToggled = State;
	OnRep_IsToggled();
}


void AInteractableButton::OnRep_IsToggled()
{
	// Change the position and material of the button depending on the toggle state
	if (bIsToggled)
	{
		Button->SetRelativeLocation(FVector(0.f, 0.f, -2.f));

		Button->SetMaterial(0, ToggledMaterial);
	}
	else
	{
		Button->SetRelativeLocation(FVector(0.f, 0.f, 2));

		Button->SetMaterial(0, UntoggledMaterial);
	}
}

void AInteractableButton::ButtonEffectOnToggle()
{
	// ...
}

void AInteractableButton::ButtonEffectOnUntoggle()
{
	// ...
}

void AInteractableButton::ClientBeginHover()
{
	// Changing the material of default mesh to hovered materail
	DefaultMesh->SetMaterial(0, HoveredMaterial);
}

void AInteractableButton::ClientEndHover()
{
	// Changing the material of default mesh to not hovered materail
	DefaultMesh->SetMaterial(0, UnhoveredMaterial);
}

void AInteractableButton::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractableButton, bIsToggled);
	DOREPLIFETIME(AInteractableButton, TiedButtonsArray);
	DOREPLIFETIME(AInteractableButton, bIsCheckable);
}