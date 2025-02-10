// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ShopWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../../../Public/Character/RBaseCharacter.h"
#include "UserWidget/Inventory/OfferedItemWidget.h"
#include "NPC/Merchant.h"
#include "UMG/Public/Components/UniformGridPanel.h"
#include "InventorySystem/Inventory.h"
#include "UMG/Public/Components/PanelSlot.h"
#include "UMG/Public/Components/UniformGridSlot.h"
#include "UserWidget/Inventory/ItemDragDropOperation.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "InventorySystem/InventoryStruct.h"
#include "UserWidget/Inventory/SellWidget.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UMG/Public/Components/WrapBox.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this, &UShopWidget::OnCloseButtonClicked);
}

void UShopWidget::OnCloseButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}


bool UShopWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent,InOperation);

	if (Cast<UItemDragDropOperation>(InOperation))
	{
		 UInventorySlotWidget* Slot= Cast<UItemDragDropOperation>(InOperation)->DraggedSlot;
	
		 if (Slot->ItemInfo.Category==EItemCategories::QuestItem)
		 {
			 return false;
		 } 
		 else
		 {
			if(!Slot->ItemInfo.bCanBeStacked && Slot->Amount>0)
			{
				if(PlayerCharacter->InventoryRef->RemoveItemAtIndex(Slot->SlotIndex,1))
				{
					PlayerCharacter->IncreaseCoin(Slot->ItemInfo.Price);
					UpdateAllItem();
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				
	
				SellWidget->Update(Slot->SlotIndex);

				SellWidget->SetVisibility(ESlateVisibility::Visible);
				PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(false);
				GridPanel->SetIsEnabled(false);

				return true;
			}
		 }
	}
	else
	{
		return false;
	}


}


void UShopWidget::UpdatCoin()
{
	CoinText->SetText(FText::AsNumber(PlayerCharacter->CurrentCoin));
}


void UShopWidget::GenerateItemList()
{
	OfferedItemWidgets.Empty();
	GridPanel->ClearChildren();
	for(int i=0; i< Merchant->OfferedItems.Num();i++)
	{
		UOfferedItemWidget* OfferedItemWidget = CreateWidget<UOfferedItemWidget>(GetWorld(), LoadClass<UOfferedItemWidget>(GetWorld(), TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_OfferedItem.WBP_OfferedItem_C'")));
		OfferedItemWidget->Init(Merchant->OfferedItems[i], PlayerCharacter->InventoryRef,this);
		OfferedItemWidgets.Add(OfferedItemWidget);
		UPanelSlot* Slot =GridPanel->AddChild(OfferedItemWidget);

		Cast<UUniformGridSlot>(Slot)->SetRow(i/2);
		Cast<UUniformGridSlot>(Slot)->SetColumn(i%2);
	}
}

void UShopWidget::UpdateAllItem()
{
	for(UOfferedItemWidget* OfferedItemWidget: OfferedItemWidgets)
	{
		OfferedItemWidget->UpdatePrice();
	}
}
