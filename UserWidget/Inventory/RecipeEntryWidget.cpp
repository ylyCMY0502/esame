// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/RecipeEntryWidget.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UserWidget/Inventory/CraftingMenuWidget.h"
#include "InventorySystem/BaseItem.h"
#include "InventorySystem/Inventory.h"

#define LOCTEXT_NAMESPACE "RecipeEntryWidget"

void URecipeEntryWidget::Update()
{
	
	IconImage->SetBrushFromTexture(Item->GetDefaultObject<ABaseItem>()->ItemInfo.Icon);
	TArray<int> ItemIndices;
	CurrentAmount=CraftingMenuWidget->Inventory->GetToalAmountOfItem(Item, ItemIndices);
	NameText->SetText(Item->GetDefaultObject<ABaseItem>()->ItemInfo.Name);
	NumText->SetText(FText::Format(LOCTEXT("RecipeEntryWidget", "{0}/{1}"), FText::AsNumber(CurrentAmount), FText::AsNumber(RequiredAmount)));

	if(CurrentAmount< RequiredAmount)
	{
		NumText->SetColorAndOpacity(FLinearColor(1,0.01,0,1));
	}
	else
	{
		NumText->SetColorAndOpacity(FLinearColor(0.43,1,0.11,1));
	}

}

#undef LOCTEXT_NAMESPACE