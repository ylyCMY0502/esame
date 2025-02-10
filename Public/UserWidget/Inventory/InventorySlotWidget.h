// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../InventorySystem/InventoryStruct.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	class UButton*SlotButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;


	UPROPERTY(meta = (BindWidget))
	class UBorder* Base;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock*AmountText;

	
protected:
	
	
	UPROPERTY(EditAnywhere,Category="Drag")
	TSubclassOf< class UDragDropOperation> DragDropOp;

public:
	//序号
	int SlotIndex;
	FItemInfo ItemInfo;
	int Amount;
	class AInventory* InventoryRef;

	bool bDraggedOver;


	class UItemDetailWidget*ItemDetailWidget;


	//class UActionMenuWidget* ActionMenuWidget;

	void UpdateSlot();

	virtual void NativeConstruct() override;

	//单
	//virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	//双击
	//virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;


	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	//拖拽UI是离开触发 
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	//当放下的时候
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;


	UFUNCTION()
	void OnSlotButtonClicked();

	int ClickCount;
	FTimerHandle TimerHandle_CountToZero;
	void SetClickCountToZero();

};



