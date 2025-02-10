// Fill out your copyright notice in the Description page of Project Settings.
#pragma warning(disable: 4458)

#include "UserWidget/Quest/QuestJournalWidget.h"
#include "Character/RBaseCharacter.h"
#include "Quest/BaseQuest.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UserWidget/Quest/GoalEntryWidget.h"
#include "Quest/QuestStruct.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "Other/StaticLibrary.h"
#include "Quest/QuestEnum.h"
#include "Quest/QuestManager.h"
#include "UserWidget/Quest/QuestListEntryWidget.h"
#include "UserWidget/Quest/QuestCategoryWidget.h"
#include "UMG/Public/Components/Button.h"
#include "UserWidget/Quest/QuestWidget.h"
#include "UserWidget/Quest/SubGoalWidget.h"

#define LOCTEXT_NAMESPACE "QuestJournalWidget"



void UQuestJournalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SelectButton->OnClicked.AddDynamic(this, &UQuestJournalWidget::OnSelectButyonClicked);

	SelectButton->OnClicked.AddDynamic(this,&UQuestJournalWidget::OnCloseButtonClicke);


}

void UQuestJournalWidget::OnCloseButtonClicke()
{
	if(SelectedQuest)
	{
		
	}
}

// 关闭窗口
void UQuestJournalWidget::CloseQuestWindow()
{
}

void UQuestJournalWidget::UpdateSuggestedLevelColor()
{
	FLinearColor TempColor; 
	TempColor= PlayerCharacter->GetCurrentLevel() > SelectedQuest->QuestInfo.SuggestedLevel ? FLinearColor::Green :
	(PlayerCharacter->GetCurrentLevel() == SelectedQuest->QuestInfo.SuggestedLevel ? FLinearColor::Blue : FLinearColor::Red );
	
	SuggestedLevel->SetColorAndOpacity(TempColor);
}

void UQuestJournalWidget::UpdateDescription()
{
	Description->SetText(FText::FromString(SelectedQuest->CurrentDescription.ToString().Replace(TEXT("/n"),TEXT("\n"))));
}

void UQuestJournalWidget::GenerateSubGoal()
{
	//��ɵ�����
	QuestGoalBox->ClearChildren();
	for (FCopmletdeGoal CompleteGoal:SelectedQuest->CompletedSubGoals)
	{
		UGoalEntryWidget* GoalEntryWidget=CreateWidget<UGoalEntryWidget>(GetWorld(),LoadClass<UGoalEntryWidget>(GetWorld(),TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Quest/WPB_GoalEntry.WPB_GoalEntry_C'")));
		GoalEntryWidget->GoalInfo= CompleteGoal.GoalInfo;
		GoalEntryWidget->Journal=this;
		GoalEntryWidget->GoalState= CompleteGoal.bSuccessful ?  EGoalState::Completed : EGoalState::Current;
		QuestGoalBox->AddChildToVerticalBox(GoalEntryWidget); 
	}
	//��ǰ������
	for(int i=0; i<SelectedQuest->CurrentGoals.Num();i++)
	//for (FGoalInfo GoalInfo :SelectedQuest->CurrentGoals)
	{
		UGoalEntryWidget* GoalEntryWidget = CreateWidget<UGoalEntryWidget>(GetWorld(), LoadClass<UGoalEntryWidget>(GetWorld(), TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Quest/WPB_GoalEntry.WPB_GoalEntry_C'")));
		GoalEntryWidget->GoalInfo = SelectedQuest->CurrentGoals[i];
		GoalEntryWidget->HuntIndex = i;
		GoalEntryWidget->Journal = this;
		GoalEntryWidget->GoalState =EGoalState::Current;
		QuestGoalBox->AddChildToVerticalBox(GoalEntryWidget);
	}

}

void UQuestJournalWidget::UpdateDetailWindow()
{
	if (SelectedQuest)
	{
		QuestName->SetText(SelectedQuest->QuestInfo.Name);


		QuestCategory->SetText(FText::FromString(UStaticLibrary::GetEnumValueAsString<EQuestCategories>("EQuestCategories",SelectedQuest->QuestInfo.Category)));
		
		FLinearColor TempColor;
		switch (SelectedQuest->QuestInfo.Category)
		{
		case EQuestCategories::MainQuest:
			TempColor=FLinearColor::Green;
			break;
		case EQuestCategories::SideQuest:
			TempColor = FLinearColor::Blue;
			break;
		case EQuestCategories::Events:
			TempColor = FLinearColor::Yellow;
			break;
		default:
			break;
		}
		QuestCategory->SetColorAndOpacity(TempColor);


		QuestRegion->SetText(FText::FromString(UStaticLibrary::GetEnumValueAsString<ERegions>("Region", SelectedQuest->QuestInfo.Region)));

		SuggestedLevel->SetText(FText::AsNumber(SelectedQuest->QuestInfo.SuggestedLevel));
		UpdateSuggestedLevelColor();

		//����Ľ���
		if (SelectedQuest->QuestInfo.CompletionReward.Experience > 0)
		{
			EXPRewarlBox->SetVisibility(ESlateVisibility::Visible);
			EXPReward->SetText(FText::Format(LOCTEXT("QuestJournalWidget","+{0} EXP"), FText::AsNumber(SelectedQuest->QuestInfo.CompletionReward.Experience)));
		} 
		else
		{
			EXPRewarlBox->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (SelectedQuest->QuestInfo.CompletionReward.Coin > 0)
		{
			GoldRewarlBox->SetVisibility(ESlateVisibility::Visible);
			GoldReward->SetText(FText::Format(LOCTEXT("QuestJournalWidget", "+{0} Gold"), FText::AsNumber(SelectedQuest->QuestInfo.CompletionReward.Coin)));
		}
		else
		{
			GoldRewarlBox->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (SelectedQuest->QuestInfo.CompletionReward.Coin <= 0  && SelectedQuest->QuestInfo.CompletionReward.Experience <= 0)
		{
			Description->SetVisibility(ESlateVisibility::Collapsed);
		}




		//���������
		UpdateDescription();

		GenerateSubGoal();


		if (SelectedQuest->CurrentState==EQuestSates::CurrentQuest)
		{
			ButtonBox->SetVisibility(ESlateVisibility::Visible);
		} 
		else
		{
			ButtonBox->SetVisibility(ESlateVisibility::HitTestInvisible);
		}



		QuestDetailsBox->SetVisibility(ESlateVisibility::Visible);
	} 
	else
	{
		QuestDetailsBox->SetVisibility(ESlateVisibility::Hidden);
		ButtonBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}



void UQuestJournalWidget::Initialize(class AQuestManager* QuestManager)
{
	this->QuestManager=QuestManager;
	PlayerCharacter=QuestManager->PlayerCharacter;
}

void UQuestJournalWidget::OnQuestClicked(class UQuestListEntryWidget* ClickedQuestListEntryWidget)
{
	if (ClickedQuestListEntryWidget)
	{
	

		if(CurrentUQuestListEntryWidget)
		{
			CurrentUQuestListEntryWidget->SetIsEnabled(true);
		}
		CurrentUQuestListEntryWidget = ClickedQuestListEntryWidget;
		SelectedQuest= CurrentUQuestListEntryWidget->AssignedQuest;
		UpdateDetailWindow();

		CurrentUQuestListEntryWidget->SetIsEnabled(false);
	}
	else
	{
		UpdateDetailWindow();
	}
}

void UQuestJournalWidget::AddQuestEntry(class ABaseQuest* Quest)
{
	UQuestListEntryWidget* QuestListEntryWidget=CreateWidget<UQuestListEntryWidget>(GetWorld(),LoadClass<UQuestListEntryWidget>(GetWorld(),TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Quest/WBP_QustListEntry.WBP_QustListEntry_C'")));
	QuestListEntryWidget->Journal=this;
	QuestListEntryWidget->AssignedQuest=Quest;
	Quest->ListEntryWidget= QuestListEntryWidget;
	AllQuestListEntries.Add(QuestListEntryWidget);

	switch (Quest->CurrentState)
	{
	case EQuestSates::CurrentQuest:
					//UVerticalBox
		CatCurrent->QuestBox->AddChild(QuestListEntryWidget);
		break;
	case EQuestSates::CompletedQuest:
		CatCompleted->QuestBox->AddChild(QuestListEntryWidget);
		break;
	case EQuestSates::FailedQuest:
		CatFailed->QuestBox->AddChild(QuestListEntryWidget);
		break;
	default:
		break;
	}
}



void UQuestJournalWidget::OnSelectButyonClicked()
{
	if(QuestManager->CurrentQuest != SelectedQuest)
	{
		//USubGoalWidget
		QuestManager->SelectNewQuest(SelectedQuest,SelectedQuest->QuestWidget->SubGoalWidgets[0]);
	}
}

#undef LOCTEXT_NAMESPACE