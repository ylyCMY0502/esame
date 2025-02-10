// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/BaseItem.h"
#include "ItemCoin.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AItemCoin : public ABaseItem
{
	GENERATED_BODY()

	virtual void OnIteractWith(ARBaseCharacter* Character) override;
	
};
