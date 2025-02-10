// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ItemDetailWidget.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Image.h"
#include "Other/StaticLibrary.h"
#include "InventorySystem/InventoryEnum.h"

void UItemDetailWidget::Update()
{
	ImageIocn->SetBrushFromTexture(ItemInfo.Icon);
	NameText->SetText(ItemInfo.Name);
	AmountText->SetText(FText::AsNumber(Amount));
	

	CategoryText->SetText(FText::FromString(UStaticLibrary::GetEnumValueAsString<EItemCategories>("EItemCategories",ItemInfo.Category)));
	DescriptionText->SetText(ItemInfo.Description);

	AmountGText->SetText(FText::AsNumber(Amount));
	SingleWeightText->SetText(FText::AsNumber(ItemInfo.Weifht));

	if (TotalWeightText)
	{
		TotalWeightText->SetText(FText::AsNumber(ItemInfo.Weifht * Amount));
	}
	


}
