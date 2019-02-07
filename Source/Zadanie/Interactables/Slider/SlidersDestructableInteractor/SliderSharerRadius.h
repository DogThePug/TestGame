// Made by Vladyslav Kulinych.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Slider/SlidersDestructableInteractor/SliderDestructableInteractor.h"
#include "SliderSharerRadius.generated.h"

/**
 *  Slider that changes the share radius of a damage sharer
 */
UCLASS()
class ZADANIE_API ASliderSharerRadius : public ASliderDestructableInteractor
{
	GENERATED_BODY()
	

protected:
	float InitialSphereRadiusMin = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "150.0", ClampMax = "250.0"))
	float InitialSphereRadiusMax = 750.f;

	// Change the radius of the damage sharing sphere
	virtual void OnRep_SliderValue() override;

};
