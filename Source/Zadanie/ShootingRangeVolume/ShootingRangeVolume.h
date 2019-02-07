// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingRangeVolume.generated.h"

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
	
	// Box
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

	UPROPERTY(EditAnywhere, Category = "Setup")
	TArray<FDestructableToLocationSpawn> DestructiblesToSpawnLocations;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
