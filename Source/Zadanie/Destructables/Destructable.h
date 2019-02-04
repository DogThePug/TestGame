// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Destructable.generated.h"

/**
* Base class for destructable actors. 
*/
UCLASS()
class ZADANIE_API ADestructable : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	ADestructable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Amount of health for this actor
	UPROPERTY(Replicated, EditAnywhere, Category = "Setup")
	float Health = 100.f;
	
	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
