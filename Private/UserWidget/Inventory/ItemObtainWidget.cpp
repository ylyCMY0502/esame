// Fill out your copyright notice in the Description page of Project Settings.
#pragma warning(disable: 4458)

#include "UserWidget/Inventory/ItemObtainWidget.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "InventorySystem/BaseItem.h"
#include "UserWidget/MainUserWidget.h"

void UItemObtainWidget::Init(TSubclassOf<class ABaseItem> ItemClass,int Amount, class UMainUserWidget* MainUserWidget)
{
	this->MainUserWidget= MainUserWidget;
	ABaseItem* Item= ItemClass->GetDefaultObject<ABaseItem>();

	ImageIocn->SetBrushFromTexture(Item->ItemInfo.Icon);
	NameText->SetText(Item->ItemInfo.Name);
	AmountText->SetText(FText::AsNumber(Amount));

	FLinearColor TempColor;

	switch (Item->ItemInfo.Category)
	{
	case EItemCategories::Consumle:
		TempColor=FLinearColor(FVector(0,0.5,1));
		break;
	case EItemCategories::Equipment:
		TempColor = FLinearColor(FVector(1, 1, 0));
		break;
	case EItemCategories::QuestItem:
		TempColor = FLinearColor(FVector(0, 1, 0));
		break;
	case EItemCategories::Readable:
		TempColor = FLinearColor(FVector(1, 0.5, 0));
		break;
	case EItemCategories::Material:
		TempColor = FLinearColor::Green;
		break;

	default:
		break;
	}

	NameText->SetColorAndOpacity(TempColor);
	AmountText->SetColorAndOpacity(TempColor);
	XText->SetColorAndOpacity(TempColor);
}
