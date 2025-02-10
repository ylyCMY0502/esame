// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/StorageSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "InventorySystem/BaseItem.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "UserWidget/Inventory/ItemDragWidget.h"
#include "UMG/Public/Blueprint/DragDropOperation.h"
#include "UserWidget/Inventory/ItemDragDropOperation.h"
#include "Character/RBaseCharacter.h"
#include "UMG/Public/Components/Border.h"
#include "UserWidget/Inventory/ItemDetailWidget.h"
#include "InventorySystem/Storage.h"
#include "UserWidget/Inventory/StorageWidget.h"
#include "UserWidget/Inventory/StorageSlotDragDropOperation.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "InventorySystem/Inventory.h"



void UStorageSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SlotButton->OnClicked.AddDynamic(this, &UStorageSlotWidget::OnSlotButtonClicked);
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





FReply UStorageSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton).NativeReply;
	}
	else
	{
		return FReply::Handled();
	}
}




void UStorageSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (StorageRef->GetItemAtIndex(SlotIndex))
	{

		UItemDragWidget* ItemDrag = CreateWidget<UItemDragWidget>(GetWorld(), LoadClass<UItemDragWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_ItemDrag.WBP_ItemDrag_C'")));



		if (ItemDrag)
		{
			ItemDrag->Init(Amount, StorageRef->GetItemAtIndex(SlotIndex)->ItemInfo.Icon);

			UDragDropOperation*StorageSlotOp= UWidgetBlueprintLibrary::CreateDragDropOperation(StorageSlotDrag);
			StorageSlotOp->DefaultDragVisual= ItemDrag;
			OutOperation= StorageSlotOp;

			if(   Cast<UStorageSlotDragDropOperation>(OutOperation)    )
			{
				Cast<UStorageSlotDragDropOperation>(OutOperation)->StorageSlotWidget=this;
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemDrag NULL"));
		}



	}
}




bool UStorageSlotWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	if (bDraggedOver)
	{
		return true;
	}
	else
	{
		if(Cast<UItemDragDropOperation>(InOperation))
		{
			Base->SetBrushColor(FLinearColor(1.0f, 0.82f, 0.0f, 0.5f));
			bDraggedOver = true;
		}
		else if(Cast<UStorageSlotDragDropOperation>(InOperation))
		{
			if(  Cast<UStorageSlotDragDropOperation>(InOperation)->StorageSlotWidget != this  )
			{
				Base->SetBrushColor(FLinearColor(1.0f, 0.82f, 0.0f, 0.5f));
				bDraggedOver = true;
			}
			else
			{
				return false;
			}
		}
		
		return false;
		
	}

}



void UStorageSlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	if (bDraggedOver)
	{
		
		if (Cast<UStorageSlotDragDropOperation>(InOperation)  || Cast<UItemDragDropOperation>(InOperation) )
		{
		
			
			bDraggedOver = false;
			Base->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

		}
	}
}




bool UStorageSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if (Cast<UStorageSlotDragDropOperation>(InOperation))
	{
		bDraggedOver = false;
		Base->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

		if (StorageRef->AddToInedx(Cast<UStorageSlotDragDropOperation>(InOperation)->StorageSlotWidget->SlotIndex,SlotIndex))
		{
			return true;
		}
		else
		{
			return StorageRef->SwapSlot(Cast<UStorageSlotDragDropOperation>(InOperation)->StorageSlotWidget->SlotIndex, SlotIndex);
		}
	} 
	else if (Cast<UItemDragDropOperation>(InOperation))
	{
		
		return  StorageRef->MoveFromInventoryToStorageByIndex(Cast<UItemDragDropOperation>(InOperation)->DraggedSlot->InventoryRef, Cast<UItemDragDropOperation>(InOperation)->DraggedSlot->SlotIndex, SlotIndex);
	}
	else
	{ 
	

		
		return false;
	
	}
}



void UStorageSlotWidget::UpdateSlot()
{
	//插槽为空
	if (StorageRef->BIsSlotEmpty(SlotIndex))
	{
		SlotButton->SetIsEnabled(false);
		IconImage->SetVisibility(ESlateVisibility::Hidden);
		AmountText->SetVisibility(ESlateVisibility::Hidden);
		SlotButton->SetToolTip(nullptr);
	}
	else
	{
		SlotButton->SetIsEnabled(true);
		ItemInfo = StorageRef->GetItemAtIndex(SlotIndex)->ItemInfo;
		Amount = StorageRef->GetAmountAtIndex(SlotIndex);
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
			ItemDetailWidget = CreateWidget<UItemDetailWidget>(GetWorld(), LoadClass<UItemDetailWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_ItemDetail.WBP_ItemDetail_C'")));
		}

		if (ItemDetailWidget)
		{
			ItemDetailWidget->ItemInfo = ItemInfo;
			ItemDetailWidget->Amount = Amount;
			//
			SlotButton->SetToolTip(ItemDetailWidget);
			ItemDetailWidget->Update();
		}

	}
}




//因为自带的双击用不了  改写成双击
void UStorageSlotWidget::OnSlotButtonClicked()
{
	ClickCount++;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CountToZero, this, &UStorageSlotWidget::SetClickCountToZero, 0.2f, false);

}

void UStorageSlotWidget::SetClickCountToZero()
{

	if (ClickCount > 1)
	{

		

		//UE_LOG(LogTemp, Warning, TEXT("ClickCount > 1"));
		ClickCount = 0;
		StorageWidget->OnSlotClicked(SlotIndex);
		Base->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

		//StorageRef->UseItemAtIndex(SlotIndex);
		//StorageRef->UpdateActionMenuPosition(this, false);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("ClickCount 00"));

		ClickCount = 0;
		//StorageRef->UpdateActionMenuPosition(this, true);
	}


	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CountToZero);
}