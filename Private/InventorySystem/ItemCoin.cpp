// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemCoin.h"

void AItemCoin::OnIteractWith(ARBaseCharacter* Character)
{
	Character->IncreaseCoin(Amount);
	Character->MainUserWidget->AddItemToObtainQueue(this->GetClass(), Amount);
	Destroy();
}
