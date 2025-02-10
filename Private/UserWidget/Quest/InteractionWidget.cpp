// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Quest/InteractionWidget.h"
#include "UMG/Public/Components/TextBlock.h"

void UInteractionWidget::SetNameText(FText Name)
{
	NameText->SetText(Name);
}

void UInteractionWidget::SetInteractionText(FText Text)
{
	InteractionText->SetText(Text);
}
