// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryStruct.h"
#include "ItemDetailWidget.generated.h"

/**
 * 
 */

 class UTextBlock;
 

UCLASS()
class RPGPROJECT_API UItemDetailWidget : public UUserWidget
{
	GENERATED_BODY()
	

	UPROPERTY(meta=(BindWidget))
	class UImage*ImageIocn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*AmountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CategoryText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionText;

	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmountGText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SingleWeightText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalWeightText;


public:
	FItemInfo ItemInfo;
	int Amount;

	void Update();

};
