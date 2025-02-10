// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Quest/SubGoalWidget.h"
#include "Components/TextBlock.h"
#include "Quest/BaseQuest.h"
#include "UserWidget/Quest/QuestWidget.h"
#include "Quest/QuestManager.h"
#include "Components/Button.h"

#define LOCTEXT_NAMESPACE "SubGoalWindget"

void USubGoalWidget::Update()
{
	//��ȡ����
	for (int i=0; i < AssignedQuest->CurrentGoals.Num();i++)
	{
		if(GoalInfo == AssignedQuest->CurrentGoals[i])
		{
			//
			HuntIndex=i;
			break;
		}
	}




	if (GoalInfo.bCustomGoal)
	{
		GoalText->SetText(GoalInfo.GoalText);
	} 
	else
	{
		FText TempText;
		FText bS;

		switch (GoalInfo.Type)
		{
		case EGoalTypes::Custom:
			GoalText->SetText(GoalInfo.GoalText);
			break;
		case EGoalTypes::Hunt:
			if (GoalInfo.AmountToHunt>1)
			{
				bS = FText::FromString("s");
			} 
			else
			{
				bS = FText::FromString("");
			}




			TempText = FText::Format(LOCTEXT("SubGoalWindget", "Hunt{0}{1}:{2}/{3}"), GoalInfo.AdditionalName, bS, FText::AsNumber(AssignedQuest->CurrentHuntedAmounts[HuntIndex]), FText::AsNumber(GoalInfo.AmountToHunt));
			break;
		case EGoalTypes::Find:
			TempText = FText::Format(LOCTEXT("SubGoalWindget", "Find the {0}"), GoalInfo.AdditionalName);

			break;
		case EGoalTypes::Talk:
			TempText = FText::Format(LOCTEXT("SubGoalWindget", "Talk the {0}"), GoalInfo.AdditionalName);
			break;
		default:
			break;
		}
		GoalText->SetText(TempText);
	}


	//����Ÿ�ֵ  ���ܵ�����==������  ��ֵ�����������Ƕ���=�����е�λ�ӱ��
	for (int i=0;i< AssignedQuest->QuestInfo.SubGoals.Num();i++)
	{
		if (AssignedQuest->QuestInfo.SubGoals[i] ==GoalInfo)
		{
			GoalIndex = i;
		}
	}

}


void USubGoalWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectButton->OnClicked.AddDynamic(this, &USubGoalWidget::OnSelectButtonClicked);
	Update();
}

void USubGoalWidget::OnSelectButtonClicked()
{
	if (QuestWidget->BCurrnetQuest())
	{
		QuestWidget->SelectSubgGoal(this);

		UE_LOG(LogTemp, Error, TEXT("OLD"));
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NEW"));
		QuestWidget->QuestManager->SelectNewQuest(AssignedQuest, this);
	}
}

#undef LOCTEXT_NAMESPACE 