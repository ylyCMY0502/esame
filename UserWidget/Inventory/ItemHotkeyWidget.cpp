// Fill out your copyright notice in the Description page of Project Settings.
#pragma warning(disable: 4458)

#include "UserWidget/Inventory/ItemHotkeyWidget.h"
#include "../../../Public/UserWidget/Inventory/InventorySlotWidget.h"
#include "../../../Public/UserWidget/Inventory/ItemDragDropOperation.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Image.h"
#include "InventorySystem/BaseItem.h"
#include "InventorySystem/Inventory.h"


void UItemHotkeyWidget::UPdate()
{
	if (bEmpty)
	{
		IconImage->SetVisibility(ESlateVisibility::Hidden);
		AmountText->SetVisibility(ESlateVisibility::Hidden);
		bDraggedOver=false;
	
		Base->SetColorAndOpacity(DefaultTint);
	} 
	else
	{
		ABaseItem*Item=Inventory->GetItemAtIndex(InvnetoryIndex);
		if (Item==nullptr)
		{
			IconImage->SetVisibility(ESlateVisibility::Hidden);
			AmountText->SetVisibility(ESlateVisibility::Hidden);
			bDraggedOver=false;
			Base->SetColorAndOpacity(DefaultTint);
		} 
		else
		{
			IconImage->SetBrushFromTexture(Item->ItemInfo.Icon);
			IconImage->SetVisibility(ESlateVisibility::Visible);
			AmountText->SetText(FText::AsNumber(Inventory->Slots[InvnetoryIndex].Amout)); //Inventory->Slots[InvnetoryIndex].Amout;
			AmountText->SetVisibility(ESlateVisibility::Visible);
		}
	}

}

FReply UItemHotkeyWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);


	//��ȡ�ǲ�������Ҽ�����   /**����ɾ������         
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && !bEmpty )
	{
		bEmpty=true;
		UPdate();
	}
	
	return FReply::Handled();
	
}

bool UItemHotkeyWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	if (bDraggedOver)
	{
		return true;
	} 
	else if (Cast<UItemDragDropOperation>(InOperation))
	{
		//�ŵ�λ�ñ����� ��ת���ɹ�
		bDraggedOver=true;
		//set ��ɫ
		Base->SetColorAndOpacity(DragOverTint);
		return true;
	}
	else
	{
		return false;
	}

}

void UItemHotkeyWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	//����ק�Ĺ�����
	if (bDraggedOver)
	{	
		//ת���ɹ�  ������ָ�����
		if (Cast<UItemDragDropOperation>(InOperation))
		{
			Base->SetColorAndOpacity(DefaultTint);
			bDraggedOver=false;
		}
	}
	
}

bool UItemHotkeyWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if (Cast<UItemDragDropOperation>(InOperation))
	{
		//�ó� ��ק�е� �����Ϣ
		UInventorySlotWidget*Slot= Cast<UItemDragDropOperation>(InOperation)->DraggedSlot;
		//��Ӧ ������ ��λ�� ������
		InvnetoryIndex=Slot->SlotIndex;
		Inventory=Slot->InventoryRef;
		bEmpty=false;
		UPdate();
		bDraggedOver=false;
		return true;
	} 
	else
	{
		return false;
	}
}

void UItemHotkeyWidget::Init(FKey Key, class AInventory* Inventory)
{
	this->Inventory = Inventory;
	this->Key=Key;
	
	HotkeyName->SetText(Key.GetDisplayName());
}

