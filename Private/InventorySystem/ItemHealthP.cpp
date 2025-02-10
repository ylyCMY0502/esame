// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemHealthP.h"
#include "InventorySystem/Inventory.h"
#include "Character/RBaseCharacter.h"

void AItemHealthP::OnUsed()
{
	if(InventoryRef->PlayerCharacter->GetCurrentHP() < InventoryRef->PlayerCharacter->GetTotalHp())
	{
		InventoryRef->PlayerCharacter->ChangeCurrentHP(HpDuration);
		InventoryRef->RemoveItemAtIndex(Index,1);
		Destroy();
	}
	else
	{
		Destroy();
	}
		
	
}
