// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemStaff.h"
#include "InventorySystem/Inventory.h"
#include "Character/RBaseCharacter.h"

void AItemStaff::OnUsed()
{
	//Super::OnUsed();
	UE_LOG(LogTemp, Warning, TEXT("OnUsed"));
	
	if(! InventoryRef->PlayerCharacter->EquipItem(this))
	{
		UE_LOG(LogTemp, Warning, TEXT("EquipItem"));
		Destroy();
	}

}
