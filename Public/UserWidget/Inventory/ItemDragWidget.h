// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryStruct.h"
#include "ItemDragWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UItemDragWidget : public UUserWidget
{
	GENERATED_BODY()


	UPROPERTY(meta = (BindWidget))
	class UTextBlock*AmountText;

	UPROPERTY(meta = (BindWidget))
	class UImage* ImageIcon;
	

public:
	int  Amount;
	FItemInfo ItemInfo;

	void Init(int Amount,UTexture2D* Iocn);
	
	
	
	//int Index;
};
