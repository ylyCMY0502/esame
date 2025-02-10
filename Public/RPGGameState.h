// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RPGGameState.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API ARPGGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	class URPGSave* RPGSaveInstance;
	FString SaveSlotName=TEXT("RPGSave");
	class ARBaseCharacter*PlayerCharacter;

	void SaveGame();
	void LoadGame();

};
