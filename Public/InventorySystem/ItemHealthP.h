// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/BaseItem.h"
#include "ItemHealthP.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AItemHealthP : public ABaseItem
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere,Category="HP")
	int HpDuration=50;

public:
	virtual void OnUsed() override;
	
};
