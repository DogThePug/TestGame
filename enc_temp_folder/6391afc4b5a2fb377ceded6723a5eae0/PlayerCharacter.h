// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ZADANIE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	// Static mesh for this character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;
public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	struct FTimerHandle TimerHandle;

	UPROPERTY(Replicated)
	class AInteractable* CurrentTarget;

	UPROPERTY(Replicated)
	class AInteractable* PreviousTarget;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Return inherited mesh of this character
	UFUNCTION()
	UStaticMeshComponent* GetStaticMesh() const;

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
private: 
	// Handles moving forward/backward
	UFUNCTION()
	void MoveForward(float Val);
	
	// Handles strafing
	UFUNCTION()
	void MoveRight(float Val);

	// Handles jumping
	UFUNCTION()
	void JumpPressed();

	UFUNCTION()
	void JumpReleased();

	// Engages interacting behavior from server
	UFUNCTION(Server, Reliable, WithValidation)
	void TryInterract();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBeginHover();

	UFUNCTION(Client, Unreliable)
	void DoWeHover();
};
