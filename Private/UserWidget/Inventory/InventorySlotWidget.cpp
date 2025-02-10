// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "../../../Public/InventorySystem/Inventory.h"
#include "InventorySystem/BaseItem.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "UserWidget/Inventory/ItemDragWidget.h"
#include "UMG/Public/Blueprint/DragDropOperation.h"
#include "UserWidget/Inventory/ItemDragDropOperation.h"
#include "Character/RBaseCharacter.h"
#include "UMG/Public/Components/Border.h"
#include "UserWidget/Inventory/ItemDetailWidget.h"
#include "UserWidget/Inventory/StorageSlotDragDropOperation.h"
#include "UserWidget/Inventory/StorageSlotWidget.h"
#include "UserWidget/Inventory/StorageWidget.h"
#include "InventorySystem/Storage.h"

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SlotButton->OnClicked.AddDynamic(this,&UInventorySlotWidget::OnSlotButtonClicked);
}


//FReply UInventorySlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
//	{
//		if(!InventoryRef->BIsSlotEmpty(SlotIndex))
//		{
//			InventoryRef->UpdateActionMenuPosition(this);
//		}
//	}
//	
//	return FReply::Handled();
//}

//FReply UInventorySlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
//
//	UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseButtonDoubleClick"));
//	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && ItemInfo.bCanBeUsed)
//	{
//		UE_LOG(LogTemp,Warning,TEXT("LeftMouseButton"));
//		InventoryRef->UseItemAtIndex(SlotIndex);
//		return FReply::Handled();
//	}
//	else
//	{
//		return FReply::Handled();
//	}
//}





FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown( InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::RightMouseButton).NativeReply;
	} 
	else
	{
		return FReply::Handled();
	}
}




void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent,  OutOperation);

	if (InventoryRef->GetItemAtIndex(SlotIndex))
	{ 
		
		UItemDragWidget* ItemDrag = CreateWidget<UItemDragWidget>(GetWorld(), LoadClass<UItemDragWidget>(nullptr,TEXT("WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_ItemDrag.WBP_ItemDrag_C'")));
		

		if(ItemDrag)
		{
			ItemDrag->Init(Amount, InventoryRef->GetItemAtIndex(SlotIndex)->ItemInfo.Icon);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemDrag NULL"));
		}


		UDragDropOperation* TempOp = UWidgetBlueprintLibrary::CreateDragDropOperation(DragDropOp);
		TempOp->DefaultDragVisual = ItemDrag;
		OutOperation = TempOp;

		if (Cast<UItemDragDropOperation>(OutOperation))
		{
			Cast<UItemDragDropOperation>(OutOperation)->bShiftDown = InventoryRef->PlayerCharacter->bShiftDown;
			Cast<UItemDragDropOperation>(OutOperation)->DraggedSlot=this;

		}
	}
}




bool UInventorySlotWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	if(bDraggedOver)
	{
		return true;
	}
	else
	{
		if(Cast<UItemDragDropOperation>(InOperation))
		{
			if (Cast<UItemDragDropOperation>(InOperation)->DraggedSlot != this)
			{
				Base->SetBrushColor(FLinearColor(1.0f, 0.82f, 0.0f, 0.5f));
				bDraggedOver = true;
			}
		
			return true;
		}
		else if(Cast<UStorageSlotDragDropOperation>(InOperation))
		{
			Base->SetBrushColor(FLinearColor(1.0f, 0.82f, 0.0f, 0.5f));
			bDraggedOver = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	
}



void UInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	if(bDraggedOver)
	{ 
		if (Cast<UItemDragDropOperation>(InOperation) || Cast<UStorageSlotDragDropOperation>(InOperation))
		{


			bDraggedOver = false;
			Base->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));


		}
	}
}




bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry,InDragDropEvent, InOperation);
	if (Cast<UItemDragDropOperation>(InOperation))
	{
		UInventorySlotWidget* Slot = Cast<UItemDragDropOperation>(InOperation)->DraggedSlot;
		if(Slot != this)
		{ 
			bDraggedOver = false;
			Base->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

			if (InventoryRef->AddToInedx(Slot->SlotIndex, SlotIndex))
			{
				return true;
			}
			else
			{
				if (Cast<UItemDragDropOperation>(InOperation)->bShiftDown)
				{
					InventoryRef->SplitStackToInedx(Slot->SlotIndex, SlotIndex,Slot->Amount/2);
				}
				else
				{
					InventoryRef->SwapSlot(Slot->SlotIndex, SlotIndex);
				}
				
			}

		}
		return true;
	} 
	else if(Cast<UStorageSlotDragDropOperation>(InOperation))
	{
		

		return	InventoryRef->MoveFromStorageToInventoryByIndex(Cast<UStorageSlotDragDropOperation>(InOperation)->StorageSlotWidget->StorageRef, Cast<UStorageSlotDragDropOperation>(InOperation)->StorageSlotWidget->SlotIndex, SlotIndex);
	
	}
	else
	{
		
		return false;
	}

}



void UInventorySlotWidget::UpdateSlot()
{
	//插槽为空
	if (InventoryRef->BIsSlotEmpty(SlotIndex))
	{
		SlotButton->SetIsEnabled(false);
		IconImage->SetVisibility(ESlateVisibility::Hidden);
		AmountText->SetVisibility(ESlateVisibility::Hidden);
		SlotButton->SetToolTip(nullptr);
	} 
	else
	{
		SlotButton->SetIsEnabled(true);
		ItemInfo=InventoryRef->GetItemAtIndex(SlotIndex)->ItemInfo;
		Amount=InventoryRef->GetAmountAtIndex(SlotIndex);
		IconImage->SetBrushFromTexture(ItemInfo.Icon);
		IconImage->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (ItemInfo.bCanBeStacked)
		{
			AmountText->SetText(FText::AsNumber(Amount));
			AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
		} 
		else
		{
			AmountText->SetVisibility(ESlateVisibility::Hidden);
		}


		// 到时候改成鼠标悬停
		if (!ItemDetailWidget)
		{
			ItemDetailWidget=CreateWidget<UItemDetailWidget>(GetWorld(),LoadClass<UItemDetailWidget>(nullptr,TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_ItemDetail.WBP_ItemDetail_C'")));
		}

		if (ItemDetailWidget)
		{
			ItemDetailWidget->ItemInfo=ItemInfo;
			ItemDetailWidget->Amount=Amount;
			//
			SlotButton->SetToolTip(ItemDetailWidget);
			ItemDetailWidget->Update();
		}

	}
}




//因为自带的双击用不了  改写成双击
void UInventorySlotWidget::OnSlotButtonClicked()
{
	ClickCount++;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CountToZero, this, &UInventorySlotWidget::SetClickCountToZero, 0.2f, false);

}

void UInventorySlotWidget::SetClickCountToZero()
{

	if (ClickCount > 1)
	{

		//UE_LOG(LogTemp, Warning, TEXT("ClickCount > 1"));
		ClickCount = 0;
		InventoryRef->UseItemAtIndex(SlotIndex); 
		InventoryRef->UpdateActionMenuPosition(this, false);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("ClickCount 00"));
		
		ClickCount = 0;
		InventoryRef->UpdateActionMenuPosition(this,true);
	}

	
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CountToZero);
}