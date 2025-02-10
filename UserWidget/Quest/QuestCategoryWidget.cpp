// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Quest/QuestCategoryWidget.h"
#include "Components/TextBlock.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/VerticalBox.h"

void UQuestCategoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	switch (CurrentState)
	{
	case EQuestSates::CurrentQuest:
		SetCategoryNames(FText::FromString(TEXT("当前任务")));
		break;
	case EQuestSates::CompletedQuest:
		SetCategoryNames(FText::FromString(TEXT("完成任务")));
		break;
	case EQuestSates::FailedQuest:
		SetCategoryNames(FText::FromString(TEXT("失败任务")));
		break;
	default:
		break;
	}

	ExpandButton->OnClicked.AddDynamic(this,&UQuestCategoryWidget::OnExpandButtonClick);
}

void UQuestCategoryWidget::OnExpandButtonClick()
{
	bExpand=!bExpand;
	FoldImage->SetRenderTransformAngle(bExpand? 90:0);
	QuestBox->SetVisibility(bExpand ? ESlateVisibility::SelfHitTestInvisible :ESlateVisibility::Collapsed);
}

void UQuestCategoryWidget::SetCategoryNames(FText CategoryNamesText)
{
	CategoryNames->SetText(CategoryNamesText);
}
