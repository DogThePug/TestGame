// Made by Vladyslav Kulinych.

#include "VolumeResetButton.h"
#include "../../ShootingRangeVolume/ShootingRangeVolume.h"

AVolumeResetButton::AVolumeResetButton()
{
	// Set functionality to uncheckable
	bIsCheckable = false;
	bCanBeToggledOff = false;
}

void AVolumeResetButton::ButtonEffectOnToggle()
{
	// Reset the field that this button is tied to
	if (ShootingRange)
	{
		ShootingRange->ServerResetField();
	}
}
