
// Fill out your copyright notice in the Description page of Project Settings.
#pragma warning(disable: 4458)

#include "UserWidget/Inventory/CraftingMenuWidget.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "InventorySystem/Inventory.h"
#include "InventorySystem/BaseItem.h"
#include "UserWidget/Inventory/RecipeEntryWidget.h"
#include "UserWidget/Inventory/CraftableItemWidget.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Image.h"

void UCraftingMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &UCraftingMenuWidget::OnCloseButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UCraftingMenuWidget::OnCloseButtonClicked);
	CraftButton->OnClicked.AddDynamic(this,&UCraftingMenuWidget::OnCraftButtonClicked);
	
}
void UCraftingMenuWidget::OnCloseButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UCraftingMenuWidget::OnCraftButtonClicked()
{
	if (Inventory->AddItem(CurrentItem,1)==0)
	{
		for (FInventorySlot Slot: CurrentItemInfo.Recipe)
		{
			Inventory->RemoveItem(Slot.ItemClass,Slot.Amout);
		}
	}
	CraftButton->SetIsEnabled(true);
}

void UCraftingMenuWidget::OnCraftableCliked(class UCraftableItemWidget* ClickedItem)
{
	if(CurrentCraftableWidget)
	{
		//原来的
		CurrentCraftableWidget->SelectedButton->SetIsEnabled(true);
		//现在的
		CurrentCraftableWidget=ClickedItem;
		CurrentCraftableWidget->SelectedButton->SetIsEnabled(false);
		UpdateDetailWindow(CurrentCraftableWidget->Item);
	}
	else
	{//没有的
		CurrentCraftableWidget=ClickedItem;
		CurrentCraftableWidget->SelectedButton->SetIsEnabled(false);
		UpdateDetailWindow(CurrentCraftableWidget->Item);
	}
}

void UCraftingMenuWidget::GenerateCraftaleList()
{
	CraftaleItemScrollBox->ClearChildren();


	for (TSubclassOf<ABaseItem>Item :Inventory->CraftableItems  )
	{
		UCraftableItemWidget*CraftableItemWidget=CreateWidget<UCraftableItemWidget>(GetWorld(),LoadClass<UCraftableItemWidget>(GetWorld(),TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_CraftableItem.WBP_CraftableItem_C'")));
		
		CraftableItemWidget->Init(this,Item);
		CraftaleItemScrollBox->AddChild(CraftableItemWidget);
	}
}

void UCraftingMenuWidget::GenerateRecipeEntrys()
{
	 RecipeEntriesList->ClearChildren();

	 RecipeEntryWidgets.Empty();

	 for (FInventorySlot Slot:CurrentItemInfo.Recipe)
	 {
		 URecipeEntryWidget* RecipeEntryWidget = CreateWidget<URecipeEntryWidget>(GetWorld(), LoadClass<URecipeEntryWidget>(GetWorld(), TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_RecipeEntry.WBP_RecipeEntry_C'")));
		 
		 RecipeEntryWidget->Item=Slot.ItemClass;
		 RecipeEntryWidget->RequiredAmount= Slot.Amout;
		 RecipeEntryWidget->CraftingMenuWidget=this;
		 RecipeEntryWidget->Update();
		 RecipeEntryWidgets.Add(RecipeEntryWidget);
		 RecipeEntriesList->AddChild(RecipeEntryWidget);
	 }
}

bool UCraftingMenuWidget::BCanBeCrafted()
{
	bool TempBool=true;
	for (URecipeEntryWidget* Recipe: RecipeEntryWidgets)
	{
		if (Recipe->CurrentAmount< Recipe->RequiredAmount)
		{
			TempBool=false;
			break;
		}
	}
	return TempBool;
}

void UCraftingMenuWidget::UpdateDetailWindow(TSubclassOf<class ABaseItem> Item)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateDetailWindow"));
	if (Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item"));
		if (Item == CurrentItem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Item == CurrentItem"));

			for (URecipeEntryWidget* Recipe: RecipeEntryWidgets)
			{
				Recipe->Update();
			}

			CraftButton->SetIsEnabled(BCanBeCrafted());
			DetailBox->SetVisibility(ESlateVisibility::Visible);
		} 
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Item != CurrentItem"));
			CurrentItem= Item;
			CurrentItemInfo=Item->GetDefaultObject<ABaseItem>()->ItemInfo;
			IconImage->SetBrushFromTexture(CurrentItemInfo.Icon);
			NameText->SetText(CurrentItemInfo.Name);
			DescriptionText->SetText(CurrentItemInfo.Description);
			GenerateRecipeEntrys();
			CraftButton->SetIsEnabled(BCanBeCrafted());
			DetailBox->SetVisibility(ESlateVisibility::Visible);
		}
	} 
	else
	{
		CurrentItem=nullptr;
		CraftButton->SetIsEnabled(false);
		DetailBox->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void UCraftingMenuWidget::IntiCraftaingMenu(class AInventory* Inventory)
{
	this->Inventory= Inventory;
	GenerateCraftaleList();
	UpdateDetailWindow(nullptr);
}

