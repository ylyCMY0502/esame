// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPGProjectGameModeBase.h"
#include "Character/RBaseCharacter.h"

ARPGProjectGameModeBase::ARPGProjectGameModeBase()
{
	ConstructorHelpers::FClassFinder<ARBaseCharacter>BP_RBaseCharacter(TEXT("Blueprint'/Game/Blueprints/Character/BP_RGideon.BP_RGideon_c'"));
	DefaultPawnClass = BP_RBaseCharacter.Class;
}
