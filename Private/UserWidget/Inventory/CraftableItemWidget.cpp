// Fill out your copyright notice in the Description page of Project Settings.

#pragma warning(disable: 4458)

#include "UserWidget/Inventory/CraftableItemWidget.h"
#include "UserWidget/Inventory/CraftingMenuWidget.h"
#include "InventorySystem/BaseItem.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Button.h"

void UCraftableItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SelectedButton->OnClicked.AddDynamic(this, &UCraftableItemWidget::OnSelectedButtonClicked);
}

void UCraftableItemWidget::Init(class UCraftingMenuWidget* CraftingMenuWidget, TSubclassOf<class ABaseItem> Item)
{
	this->CraftingMenuWidget= CraftingMenuWidget;
	this->Item= Item;

	NameText->SetText(Item->GetDefaultObject<ABaseItem>()->ItemInfo.Name);
}

void UCraftableItemWidget::OnSelectedButtonClicked()
{
	CraftingMenuWidget->OnCraftableCliked(this);
}
