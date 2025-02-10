// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Quest/GoalEntryWidget.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UserWidget/Quest/QuestJournalWidget.h"
#include "Quest/BaseQuest.h"
#include "UMG/Public/Components/Image.h"


#define LOCTEXT_NAMESPACE "GoalEntryWidget"

void UGoalEntryWidget::Update()
{	
	//是不是有任务
	if(GoalInfo.bCustomGoal)
	{
		SetGoalText(GoalInfo.GoalText);
	}
	else
	{
		FText TempText;
		FText bS;
		int TempHuntedAmount = 0;

		switch (GoalInfo.Type)
		{
		case EGoalTypes::Custom:
			TempText=GoalInfo.GoalText;
			break;

		case EGoalTypes::Hunt:
			if (GoalInfo.AmountToHunt>1)
			{	
				bS=FText::FromString("s");
			}
			else 
			{
				bS = FText::FromString("");
			}

			if (GoalState ==EGoalState::Failed)
			{
				TempHuntedAmount=0;
			} 
			else if(GoalState == EGoalState::Completed)
			{
				TempHuntedAmount=GoalInfo.AmountToHunt;
			}
			else
			{
				TempHuntedAmount = Journal->SelectedQuest->CurrentHuntedAmounts[HuntIndex];
			}

			TempText=FText::Format(LOCTEXT("GoalEntryWidget","Hunt{0}{1}:{2}/{3}"),GoalInfo.AdditionalName,bS,
			FText::AsNumber(TempHuntedAmount),
			FText::AsNumber(GoalInfo.AmountToHunt));
			break;

		case EGoalTypes::Find:
			TempText = FText::Format(LOCTEXT("GoalEntryWidget", "find the {0}"),GoalInfo.AdditionalName);
			break;

		case EGoalTypes::Talk:
			TempText = FText::Format(LOCTEXT("GoalEntryWidget", "Talk to {0}"), GoalInfo.AdditionalName);
			break;
		default:
			break;
		}

		SetGoalText(TempText);
	}
	UTexture2D* TempTexture=nullptr;
	switch (GoalState)
	{
	case EGoalState::Current:
		TempTexture=CurrentUTexture;
		break;

	case EGoalState::Completed:
		TempTexture=SuccessUTexture;
		break;
	case EGoalState::Failed:
		TempTexture=FailedUTexture;
		break;
	default:
		break;
	}
	GoalStateImage->SetBrushFromTexture(TempTexture); 
}

void UGoalEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Update();
}

void UGoalEntryWidget::SetGoalText(FText Text)
{
	GoalText->SetText(Text);
}

#undef LOCTEXT_NAMESPACE