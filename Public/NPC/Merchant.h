// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/BsaeNPC.h"
#include "Merchant.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AMerchant : public ABsaeNPC
{
	GENERATED_BODY()
	
	virtual void BeginPlay()override;

	virtual void OnIteractWith(ARBaseCharacter* Character) override;

	virtual void OnLeavePlayerRadius(ARBaseCharacter* Character) override;

public:

	class ARBaseCharacter*PlayerCharacter;
	class UShopWidget* ShopWidget;
	bool bShopOpen;

	UPROPERTY(EditAnywhere,Category="Item")
	TArray<TSubclassOf<class ABaseItem>>OfferedItems;


};
