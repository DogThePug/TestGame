// Made by Vladyslav Kulinych.

#include "Destructable.h"

// Sets default values
ADestructable::ADestructable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADestructable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestructable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

