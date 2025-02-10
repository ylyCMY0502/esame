// Fill out your copyright notice in the Description page of Project Settings.
#pragma warning(disable: 4458)

#include "UserWidget/Inventory/ItemDragWidget.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Image.h"
#include "Engine/Texture2D.h"

void UItemDragWidget::Init(int Amount, UTexture2D* Iocn)
{	
	if (Amount>1)
	{
		AmountText->SetText(FText::AsNumber(Amount));
		AmountText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{	
		AmountText->SetVisibility(ESlateVisibility::Hidden);
	}
	
	ImageIcon->SetBrushFromTexture(Iocn);
}
