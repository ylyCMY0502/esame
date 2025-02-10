// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameState.h"
#include "SaveGames/RPGSave.h"
#include "Character/RBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/Button.h"
#include "InventorySystem/Inventory.h"
#include "InventorySystem/Storage.h"
#include "UserWidget/MainUserWidget.h"
#include "NPC/StorageKeeper.h"
#include "UserWidget/Inventory/InventoryWidget.h"

void ARPGGameState::SaveGame()
{
	if (!RPGSaveInstance)
	{
		RPGSaveInstance=Cast<URPGSave>(UGameplayStatics::CreateSaveGameObject(URPGSave::StaticClass()));
	}

	

	if (PlayerCharacter)
	{

		RPGSaveInstance->SavedName = PlayerCharacter->GetCharacterName();
		RPGSaveInstance->SavedHP = PlayerCharacter->GetCurrentHP();
		RPGSaveInstance->SavedMP = PlayerCharacter->GetCurrentMP();
		RPGSaveInstance->SavedEXP = PlayerCharacter->GetCurrentExp();
		RPGSaveInstance->SavedLevel = PlayerCharacter->GetCurrentLevel();
		RPGSaveInstance->SavedCoin = PlayerCharacter->CurrentCoin;
		RPGSaveInstance->DefaultSpeed = PlayerCharacter->GetDefaultSpeed();
		RPGSaveInstance->PlayerLocation= PlayerCharacter->GetActorLocation();

		UGameplayStatics::SaveGameToSlot(RPGSaveInstance, SaveSlotName, 0);

		//°ü¸¤
		if (PlayerCharacter->InventoryRef)
		{
			PlayerCharacter->InventoryRef->SaveInventory();
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("PlayerCharacter nill"));
	}

	//²Ö¿â
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStorage::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		AStorage* Storage = Cast<AStorage>(Actor);
		if (Storage)
		{
			Storage->SaveStorage();
		}

	}

}


void ARPGGameState::LoadGame()
{
	if (!RPGSaveInstance)
	{
		RPGSaveInstance = Cast<URPGSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	
	if (PlayerCharacter)
	{
		PlayerCharacter->SetCharacterName(RPGSaveInstance->SavedName);
		PlayerCharacter->SetCurrentHP(RPGSaveInstance->SavedHP);
		PlayerCharacter->SetCurrentMP(RPGSaveInstance->SavedMP);
		PlayerCharacter->SetCurrentExp(RPGSaveInstance->SavedEXP);
		PlayerCharacter->SetCurrentLevel(RPGSaveInstance->SavedLevel);
		PlayerCharacter->SetDefaultSpeed(RPGSaveInstance->DefaultSpeed);
		PlayerCharacter->MainUserWidget->InventoryWidget->UpdateCoinText(RPGSaveInstance->SavedCoin);
		 PlayerCharacter->SetActorLocation(RPGSaveInstance->PlayerLocation);


		//--------°ü¸¤----
		PlayerCharacter->MainUserWidget->SetHPProgressBar(PlayerCharacter->GetCurrentHP() / PlayerCharacter->TotalHp);
		PlayerCharacter->MainUserWidget->SetMPProgressBar(PlayerCharacter->GetCurrentMP() / PlayerCharacter->TotalMp);

		if (PlayerCharacter->InventoryRef)
		{ 
			PlayerCharacter->InventoryRef->LoadInventory();
			if (PlayerCharacter->InventoryRef->CurrentWeight > PlayerCharacter->InventoryRef->TotalWeight)
			{
				PlayerCharacter->InventoryRef->bOverload = true;
				PlayerCharacter->OnOverloaded();
			}
		}
		//----------²Ö¿â
		if(	PlayerCharacter)
		{

			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStorageKeeper::StaticClass(), Actors);
			for (AActor* Actor : Actors)
			{
				AStorageKeeper* StorageKeeper = Cast<AStorageKeeper>(Actor);

				StorageKeeper->OnIteractWith(PlayerCharacter);
				PlayerCharacter->StorageRef->CloseStorage();
			}
			
			PlayerCharacter->StorageRef->LoadStorage();
		}


	}




	




}
