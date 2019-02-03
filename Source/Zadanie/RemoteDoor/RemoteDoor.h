// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RemoteDoor.generated.h"

/**
*	A door with remote unlocking functionality
*/
UCLASS()
class ZADANIE_API ARemoteDoor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DoorMesh;
public:	
	// Sets default values for this actor's properties
	ARemoteDoor();

	// Function to initiate door opening/closing
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInitiateStateChange();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Is this door open or not
	UPROPERTY(Replicated)
	bool bIsOpen = false;

	// Is this door in transition
	UPROPERTY(Replicated)
	bool bInTransition = false;

	// Distance to which the door will slide when is opened
	UPROPERTY(Replicated, EditAnywhere, Category = "BehaviorSetup")
	float SlidingDistance = 200.f;

	// Speed at which the door will slide per frame when opens
	UPROPERTY(Replicated, EditAnywhere, Category = "BehaviorSetup")
	float SlidingSpeed = 5.f;

	// Can this door be closed once was opened
	UPROPERTY(Replicated, EditAnywhere, Category = "BehaviorSetup")
	bool bCanBeClosed = true;

	// Direction where the door is moving
	UPROPERTY(Replicated)
	FVector CurrentDoorDirection;
};
