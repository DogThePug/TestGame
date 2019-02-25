// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingRangeVolume.generated.h"

// Relations of destructable class that we will be spawning, relative transform where we will be spawning it and to which sliders we need to bind the destructable
USTRUCT(Blueprintable)
struct FDestructableToLocationSpawn
{
	GENERATED_BODY()

public:
	// Destructable that this volume will spawn on reset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADestructable> Destructable;

	// Relatie transform to spawn to
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform SpawnTransform;

	// Sliders to tie this destructable with
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class ASliderDestructableInteractor*> SliderInteractors;
};

/**
* Volume that will spawn destructables on the given relative locations and tie them to the proper interactables
*/
UCLASS()
class ZADANIE_API AShootingRangeVolume : public AActor
{
	GENERATED_BODY()
	
	// Volume that will remove everything destructable in it once reset
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* VolumeBox;

public:	
	// Sets default values for this actor's properties
	AShootingRangeVolume();

	// Function to reset the field 
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerResetField();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Destructables that will spawn at given relative locations and with given sliders/interactables to tie them with
	UPROPERTY(EditAnywhere, Category = "Setup")
	TArray<FDestructableToLocationSpawn> DestructiblesToSpawnLocations;
};
