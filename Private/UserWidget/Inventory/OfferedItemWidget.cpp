// Fill out your copyright notice in the Description page of Project Settings.
#pragma warning(disable: 4458)

#include "UserWidget/Inventory/OfferedItemWidget.h"
#include "UMG/Public/Components/TextBlock.h"
#include "InventorySystem/Inventory.h"
#include "Character/RBaseCharacter.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/Slider.h"
#include "UMG/Public/Components/EditableText.h"
#include "InventorySystem/BaseItem.h"
#include "UserWidget/Inventory/ShopWidget.h"

void UOfferedItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AmountSlider->OnValueChanged.AddDynamic(this, &UOfferedItemWidget::OnAmountSliderValueChanged);
	BuyButton->OnClicked.AddDynamic(this, &UOfferedItemWidget::OnBuyButtonClicked);
	//AmountText->OnTextCommitted
	AmountText->OnTextCommitted.AddDynamic(this, &UOfferedItemWidget::OnAmountTextCommitted);
}

void UOfferedItemWidget::Init(TSubclassOf<class ABaseItem>Item, AInventory* Inventory, class UShopWidget* ShopWidget)
{
	AssignedItem= Item;
	//-------------
	this->Inventory=Inventory;
	this->ShopWidget= ShopWidget;

	ItemInfo=AssignedItem->GetDefaultObject<ABaseItem>()->ItemInfo;
	NameText->SetText(ItemInfo.Name);

	IconImage->SetBrushFromTexture(ItemInfo.Icon);
	
	
	if (ItemInfo.bCanBeStacked)
	{
		AmountText->SetText(FText::AsNumber(CurrentAmount));
		AmountSlider->SetIsEnabled(true);
	} 
	else
	{
		CurrentAmount=1;
		AmountText->SetText(FText::AsNumber(CurrentAmount));
		AmountSlider->SetIsEnabled(false);
	}
	
	
	UpdatePrice();

}


void UOfferedItemWidget::UpdatePrice()
{
	PriceText->SetText(FText::AsNumber(ItemInfo.Price * CurrentAmount));
	FLinearColor PriceColor;

	if (Inventory->PlayerCharacter->CurrentCoin > ItemInfo.Price * CurrentAmount)
	{
		BuyButton->SetVisibility(ESlateVisibility::Visible);
		PriceColor=FLinearColor::Green;
	} 
	else
	{
		BuyButton->SetVisibility(ESlateVisibility::Hidden);
		PriceColor = FLinearColor::Red;
	}
	PriceText->SetColorAndOpacity(PriceColor);
}

void UOfferedItemWidget::OnAmountSliderValueChanged(float Value)
{
	
	CurrentAmount=FMath::Lerp(1,MaxAmount,Value);
	
	AmountText->SetText(FText::AsNumber(CurrentAmount));

	UpdatePrice();
}

void UOfferedItemWidget::OnBuyButtonClicked()
{
	//我想整理了在添加
	int LeftAmount=Inventory->AddItem(AssignedItem,CurrentAmount);
	UE_LOG(LogTemp, Warning, TEXT("LeftAmount%d"), LeftAmount);
	if (LeftAmount<CurrentAmount)
	{
		UE_LOG(LogTemp, Warning, TEXT("L--CurrentAmoun%d"), (CurrentAmount - LeftAmount) * ItemInfo.Price);
		Inventory->PlayerCharacter->DecreaseCoin((CurrentAmount-LeftAmount)*ItemInfo.Price);
		
		UpdatePrice();

		ShopWidget->UpdateAllItem();
	}

	
}

void UOfferedItemWidget::OnAmountTextCommitted(const FText& Text, ETextCommit::Type Default)
{
	int Value= FCString::Atoi(*(Text.ToString()));
	if(Value> MaxAmount)
	{
		Value= MaxAmount;
	}
	AmountText->SetText(FText::AsNumber(Value));

	UpdatePrice();


	float ValueF = float(Value) / MaxAmount;

	AmountSlider->SetValue(ValueF);
	//OnAmountSliderValueChanged(ValueF);
}
