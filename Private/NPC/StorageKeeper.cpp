// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/StorageKeeper.h"
#include "InventorySystem/Storage.h"
#include "Character/RBaseCharacter.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/StorageWidget.h"

void AStorageKeeper::BeginPlay()
{
	Super::BeginPlay();



}



void AStorageKeeper::OnIteractWith(ARBaseCharacter* Character)
{
	
	if (StorageRef == nullptr)
	{
		FActorSpawnParameters Parameter;
		Parameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (StorageClass)
		{
			StorageRef = GetWorld()->SpawnActor<AStorage>(StorageClass, Parameter);
			//UE_LOG(LogTemp, Warning, TEXT("AStorageKeeperS"));


		}
	}


	//StorageRef->StorageWidget->GenerateSlotWidget(StorageRef);
	Character->StorageRef = StorageRef;
	Character->MainUserWidget->StorageWidget->GenerateSlotWidget(StorageRef);

	Character->MainUserWidget->StorageWidget->UpdateThisSlots();
	StorageRef->OpenStorage();

}

void AStorageKeeper::OnLeavePlayerRadius(ARBaseCharacter* Character)
{
	Super::OnLeavePlayerRadius(Character);

	if (StorageRef)
	{
		StorageRef->CloseStorage();
	}
	
}
