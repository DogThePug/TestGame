// Made by Vladyslav Kulinych.

#include "ShootingRangeVolume.h"
#include "Components/BoxComponent.h"
#include "../Destructables/Destructable.h"
#include "../Interactables/Slider/SlidersDestructableInteractor/SliderDestructableInteractor.h"
#include "TimerManager.h"

// Sets default values
AShootingRangeVolume::AShootingRangeVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	VolumeBox = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	RootComponent = VolumeBox;
	VolumeBox->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AShootingRangeVolume::BeginPlay()
{
	Super::BeginPlay();
	


	if (Role == ROLE_Authority)
	{
		FTimerHandle TimerHandle;

		GetWorldTimerManager().SetTimer(TimerHandle, this, &AShootingRangeVolume::ServerResetField, 0.1f, false);
	}
}

// Called every frame
void AShootingRangeVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AShootingRangeVolume::ServerResetField_Implementation()
{
	if (Role == ROLE_Authority)
	{
		// Find overlapping destructables and destroy them
		TArray<AActor*> OverlappedActors;
		GetOverlappingActors(OverlappedActors);

		for (auto Actor : OverlappedActors)
		{
			ADestructable* PossibleDestructable = Cast<ADestructable>(Actor);
			if (PossibleDestructable)
			{
				PossibleDestructable->Destroy();
			}
		}

		// Spawn all destructables from the list 
		for (FDestructableToLocationSpawn DestructableToSpawnLocation : DestructiblesToSpawnLocations)
		{
			if (DestructableToSpawnLocation.Destructable)
			{
				FTransform TransformToSpawn = DestructableToSpawnLocation.SpawnTransform;
				TransformToSpawn.SetLocation(TransformToSpawn.GetLocation() + GetActorLocation());

				ADestructable* SpawnedDestructable = GetWorld()->SpawnActor<ADestructable>(DestructableToSpawnLocation.Destructable, TransformToSpawn);
				
				// Tie the interactables to spawned destructable
				if (DestructableToSpawnLocation.SliderInteractors.Num() != 0)
				{
					for (auto SliderInteractor : DestructableToSpawnLocation.SliderInteractors)
					{
						if (SliderInteractor)
						{
							SliderInteractor->ServerBindWithDestructable(SpawnedDestructable);
						}
					}
				}
			}
		}
	}
}

bool AShootingRangeVolume::ServerResetField_Validate()
{
	return true;
}



