// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()



	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock*CoinText;

	
	
	virtual void NativeConstruct()override;

	UFUNCTION()
	void OnCloseButtonClicked();

	

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

public:
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* GridPanel;

	UPROPERTY(meta = (BindWidget))
	class USellWidget* SellWidget;

	class ARBaseCharacter* PlayerCharacter;
	class AMerchant* Merchant;
	TArray<class UOfferedItemWidget*> OfferedItemWidgets;
	
	void UpdatCoin();

	void GenerateItemList();

	void UpdateAllItem();

};
