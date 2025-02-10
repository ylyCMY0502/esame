// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ActionMenuWidget.h"
#include "../../../Public/InventorySystem/Inventory.h"
#include "../../../Public/InventorySystem/BaseItem.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../../../Public/UserWidget/Inventory/InventorySlotWidget.h"
#include "UserWidget/Inventory/ThrowWidget.h"
#include "UserWidget/MainUserWidget.h"
#include "Character/RBaseCharacter.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UMG/Public/Components/WrapBox.h"


void UActionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UseItemButton->OnClicked.AddDynamic(this,&UActionMenuWidget::OnUseItemButtonClicked);
	ThrowAwayButton->OnClicked.AddDynamic(this, &UActionMenuWidget::OnThrowAwayButtonClicked);
	SplitStacksButton->OnClicked.AddDynamic(this, &UActionMenuWidget::OnSplitStacksButtonClicked);
	CancleButton->OnClicked.AddDynamic(this, &UActionMenuWidget::OnCancleButtonClicked);
}

void UActionMenuWidget::Update(int Index)
{
	if (! InventoryRef->BIsSlotEmpty(Index))
	{
	
		CurrentIdex = Index;
		ItemInfo = InventoryRef->GetItemAtIndex(Index)->ItemInfo;
		Amount = InventoryRef->GetItemAtIndex(Index)->Amount;

		if (ItemInfo.bCanBeUsed)
		{
			UseText->SetText(ItemInfo.UseText);
			UseItemButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UseItemButton->SetVisibility(ESlateVisibility::Collapsed);

		}

		if (Amount > 1 && ItemInfo.bCanBeStacked)
		{
			SplitStacksButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			SplitStacksButton->SetVisibility(ESlateVisibility::Collapsed);
		}

	}
	
}

void UActionMenuWidget::OnUseItemButtonClicked()
{
	InventoryRef->UseItemAtIndex(CurrentIdex);
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UActionMenuWidget::OnThrowAwayButtonClicked()
{
	if (! ItemInfo.bCanBeStacked && Amount>0)
	{
		InventoryRef->RemoveItemAtIndex(CurrentIdex,1);
		//InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(true);
	} 
	else
	{
		InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(false);
		InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->CloseButton->SetIsEnabled(false);
		InventoryRef->PlayerCharacter->MainUserWidget->ThrowWidget->Update(CurrentIdex);
		InventoryRef->PlayerCharacter->MainUserWidget->ThrowWidget->SetVisibility(ESlateVisibility::Visible);
		
	}



	this->SetVisibility(ESlateVisibility::Hidden);
}

void UActionMenuWidget::OnSplitStacksButtonClicked()
{
	InventoryRef->SplitStacke(CurrentIdex, int(Amount / 2));
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UActionMenuWidget::OnCancleButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}
