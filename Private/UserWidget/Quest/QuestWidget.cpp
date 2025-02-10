// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Quest/QuestWidget.h"
#include "Components/VerticalBox.h"
#include "Quest/QuestStruct.h"
#include "Quest/BaseQuest.h"
#include "Quest/QuestManager.h"
#include "UserWidget/Quest/SubGoalWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"

void UQuestWidget::GenerateSubWidgets()
{
	SubGoalWidgets.Empty();

	SubGoalBox->ClearChildren();

	for (FGoalInfo GoalInfo: AssignedQuest->CurrentGoals)
	{
		USubGoalWidget* SubGoalWidget = CreateWidget<USubGoalWidget>(GetWorld(), LoadClass<USubGoalWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/Blueprints/UI/Quest/WBP_SubQuest.WBP_SubQuest_c'")));
		SubGoalWidget->GoalInfo = GoalInfo;
		SubGoalWidget->AssignedQuest = AssignedQuest;
		SubGoalWidget->QuestWidget = this;

		SubGoalWidgets.Add(SubGoalWidget);
		SubGoalBox->AddChild(SubGoalWidget);
	}

}

void UQuestWidget::UpdataQuest()
{
	if (AssignedQuest)
	{
		QuestName->SetText(AssignedQuest->QuestInfo.Name);

		//设置任务颜色
		FLinearColor TempColor;
		switch (AssignedQuest->QuestInfo.Category)
		{
		case EQuestCategories::MainQuest:
			TempColor = FLinearColor(1, 0.37, 0,1);//橙色
			break;
		case EQuestCategories::SideQuest:
			TempColor = FLinearColor(0.45, 0.47, 1,1);//淡蓝色
			break;
		case EQuestCategories::Events:
			TempColor = FLinearColor(1, 0.19, 0.13,1);//淡红色
			break;
		default:
			break;
		}

		QuestName->SetColorAndOpacity(TempColor);
		GenerateSubWidgets();
	}
}

bool UQuestWidget::BCurrnetQuest()
{
	return (AssignedQuest == QuestManager->CurrentQuest);
}

void UQuestWidget::SelectSubgGoal(class USubGoalWidget* ClickeSubgGoalWidget)
{
	if (SelectedSubgGoalWidget )
	{
		SelectedSubgGoalWidget->GoalBorder->SetContentColorAndOpacity(FLinearColor(1, 1, 1, 0.5));
		SelectedSubgGoalWidget->SelectButton->SetIsEnabled(true);
	} 
	

	if (ClickeSubgGoalWidget)
	{
		SelectedSubgGoalWidget = ClickeSubgGoalWidget;
		SelectedSubgGoalWidget->GoalBorder->SetContentColorAndOpacity(FLinearColor(1, 1, 1, 1));
		SelectedSubgGoalWidget->SelectButton->SetIsEnabled(false);

		AssignedQuest->SelectedSubgGoalIndex = SelectedSubgGoalWidget->GoalIndex;
		
		QuestManager->OnSwitchSubQuest();
	}
	else
	{
		SelectedSubgGoalWidget = nullptr;
	}
}

void UQuestWidget::OnQuestSelect(class USubGoalWidget* SubgGoalWidget)
{
	QuestName->SetIsEnabled(true);
	SelectSubgGoal(SubgGoalWidget);
}


