// Made by Vladyslav Kulinych.

#include "ShrinkingDestructable.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Components/TextRenderComponent.h"

#include "UnrealNetwork.h"

AShrinkingDestructable::AShrinkingDestructable()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Meshes/SM_Shape_Cube.SM_Shape_Cube'"));

	if (MeshAsset.Object)
	{
		DefaultMesh->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("Material'/Game/Materials/M_ShrinkableDestructable.M_ShrinkableDestructable'"));

	if (MaterialAsset.Object)
	{
		DefaultMesh->SetMaterial(0, MaterialAsset.Object);

		HealthyMaterial = MaterialAsset.Object;
	}

	DefaultMesh->SetWorldScale3D(InitialScale);

	HealthText->SetRelativeLocation(FVector(0.f, 11.f, 133.f));


	static ConstructorHelpers::FObjectFinder<UMaterialInstance> SlightlyHurtMaterialAsset(TEXT("MaterialInstanceConstant'/Game/Materials/M_ShrinkableDestructable_Inst.M_ShrinkableDestructable_Inst'"));

	if (SlightlyHurtMaterialAsset.Object)
	{
		SlightlyHurtMaterial = (UMaterial*)SlightlyHurtMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BadlyInjuredMaterialAsset(TEXT("MaterialInstanceConstant'/Game/Materials/M_ShrinkableDestructable_Inst_2.M_ShrinkableDestructable_Inst_2'"));

	if (BadlyInjuredMaterialAsset.Object)
	{
		BadlyInjuredMaterial = (UMaterial*)BadlyInjuredMaterialAsset.Object;
	}
}

void AShrinkingDestructable::ServerSetInitialScale_Implementation(FVector Scale)
{
	InitialScale = Scale;
	OnRep_InitialScale();
}

bool AShrinkingDestructable::ServerSetInitialScale_Validate(FVector Scale)
{
	return true;
}
void AShrinkingDestructable::OnRep_InitialScale()
{
	SetScaleBasedOnMissingHealth();
}

void AShrinkingDestructable::SetScaleBasedOnMissingHealth()
{
	// Calculating new scale depending on the amount of hp and seting it
	FVector NewScale(InitialScale * 0.2f + InitialScale * 0.8f * (Health / InitialHealth));
	SetActorScale3D(NewScale);
}

void AShrinkingDestructable::SetMaterialBasedOnMissingHealth()
{
	if (Health / InitialHealth >= 0.7f)
	{
		if (InitialMaterials.Num() != 0)
		{
			InitialMaterials[0] = HealthyMaterial;
			DefaultMesh->SetMaterial(0, HealthyMaterial);
		}
	}
	else if (Health / InitialHealth >= 0.3f)
	{
		if (InitialMaterials.Num() != 0)
		{
			InitialMaterials[0] = SlightlyHurtMaterial;
			DefaultMesh->SetMaterial(0, SlightlyHurtMaterial);
		}
	}
	else
	{
		if (InitialMaterials.Num() != 0)
		{
			InitialMaterials[0] = BadlyInjuredMaterial;
			DefaultMesh->SetMaterial(0, BadlyInjuredMaterial);
		}
	}
}

void AShrinkingDestructable::OnRep_Health()
{
	Super::OnRep_Health();

	SetScaleBasedOnMissingHealth();


	SetMaterialBasedOnMissingHealth();
	
}

void AShrinkingDestructable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShrinkingDestructable, InitialScale);
}