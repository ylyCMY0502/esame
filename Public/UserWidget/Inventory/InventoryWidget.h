// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"


class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	


	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseButtonClicke();

	
protected:

	UPROPERTY(EditAnywhere,Category="UI")
	TSubclassOf<class UDragDropOperation>InventoryDragDropOperation;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

public:

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SortButton;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString*SortCategoryBox;

	UPROPERTY(meta = (BindWidget))
	class UWrapBox* InventoryBox;



	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentlWeightText;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentlWeightTextG;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalWeightText;



	void UpdateWeight(float CurrentWeight,float TotalWeight);



	UPROPERTY(meta = (BindWidget))
	class UActionMenuWidget* ActionMenu;

	class AInventory* InventoryRef;

	TArray<class UInventorySlotWidget*> InventorySlotWidgets;
	void GenerateSlotWidget();

	UFUNCTION()
	void OnSortButtonClicked();

	bool bReversed = false;

	void UpdateCoinText(int Coin);

	void UpdateThisSlots();
};


