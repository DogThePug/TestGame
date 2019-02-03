// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "RemoteDoor/RemoteDoor.h"
#include "PuzzleRemoteDoor.generated.h"

/**
 *  Remote door with puzzle implementation
 */
UCLASS()
class ZADANIE_API APuzzleRemoteDoor : public ARemoteDoor
{
	GENERATED_BODY()
	
protected:
	// Subscribe to the change of state in
	virtual void BeginPlay() override;

private:
	// Array of lamps to listen to. When all of them will be off, this door will open
	UPROPERTY(Replicated, EditAnywhere, Category = "BehaviorSetup")
	TArray<class AInteractableLamp*> ListenedToLamps;

	// Checking if the conditions to open the door have been met
	UFUNCTION()
	void CheckConditionsMet();

	// Network Setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
};
