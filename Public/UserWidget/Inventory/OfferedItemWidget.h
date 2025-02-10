// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryStruct.h"
#include "OfferedItemWidget.generated.h"

/**
 * 
 */
class UTextBlock;

UCLASS()
class RPGPROJECT_API UOfferedItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock*NameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock*PriceText;
	UPROPERTY(meta = (BindWidget))
	class UButton*BuyButton;
	UPROPERTY(meta = (BindWidget))
	class UEditableText*AmountText;
	UPROPERTY(meta = (BindWidget))
	class USlider*AmountSlider;
	UPROPERTY(meta = (BindWidget))
	class UImage*IconImage;

	virtual void NativeConstruct()override;

public:
	class AInventory*Inventory;
	TSubclassOf<class ABaseItem>AssignedItem;
	FItemInfo ItemInfo;
	int CurrentAmount=1;

	UPROPERTY(EditAnywhere,Category="ItemInfo")
	int MaxAmount=99;

	class UShopWidget* ShopWidget;

	void Init(TSubclassOf<class ABaseItem>Item, class AInventory* Inventory, class UShopWidget* ShopWidget);
	void UpdatePrice();

	UFUNCTION()
	void OnAmountSliderValueChanged(float Value);

	UFUNCTION()
	void OnBuyButtonClicked();

	UFUNCTION()
	void OnAmountTextCommitted(const FText & Text, ETextCommit::Type OnEnter);

};
