// Made by Vladyslav Kulinych.

#include "VolumeResetButton.h"
#include "../../ShootingRangeVolume/ShootingRangeVolume.h"

AVolumeResetButton::AVolumeResetButton()
{
	bIsCheckable = false;
	bCanBeToggledOff = false;
}

void AVolumeResetButton::ButtonEffectOnToggle()
{
	if (ShootingRange)
	{
		ShootingRange->ServerResetField();
	}
}
