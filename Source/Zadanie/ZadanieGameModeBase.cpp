// Fill out your copyright notice in the Description page of Project Settings.

#include "ZadanieGameModeBase.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerHUD.h"
AZadanieGameModeBase::AZadanieGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();

	HUDClass = APlayerHUD::StaticClass();
}