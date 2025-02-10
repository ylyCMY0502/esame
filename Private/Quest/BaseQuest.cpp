// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/BaseQuest.h"
#include "Quest/QuestStruct.h"
#include "UserWidget/Quest/QuestWidget.h"
#include "UserWidget/Quest/SubGoalWidget.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "Quest/QuestManager.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Quest/QuestJournalWidget.h"
#include "UserWidget/Quest/QuestListEntryWidget.h"

// Sets default values
ABaseQuest::ABaseQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseQuest::BeginPlay()
{
	Super::BeginPlay();
	CaculateGoalAmount();
}


//���ڸ��� ��ǰ���������
void ABaseQuest::UpdateSubGoal()
{
	CurrentGoals.Empty();

	
	for (int i=0;i< CurrentGoalIndices.Num();i++)
	{	
		CurrentGoals.Add(QuestInfo.SubGoals[CurrentGoalIndices[i]]);
	}
}





void ABaseQuest::SetupStartinGoals()
{
	CurrentGoalIndices.Empty();
	CurrentGoalIndices = StartingSubGoalIndices;
	UpdateSubGoal();
	CurrentDescription=QuestInfo.Description;
	//��������ĳ���
	CurrentHuntedAmounts.SetNum(CurrentGoalIndices.Num());
}



bool ABaseQuest::OnSubGoalCompleted(int SubGoalIndex,bool bSuccess)
{
	//
	if (CurrentGoalIndices.Contains(SubGoalIndex))
	{
		CurrentGoalCount++;

		//��ȡ������Ӧ������
		FGoalInfo CompletedGoal = QuestInfo.SubGoals[SubGoalIndex];
		

		//���»�����һ������ĸ���
		if(CompletedGoal.bUpdateQuestDescription  && bSuccess)
		{
			CurrentDescription=FText::FromString(CurrentDescription.ToString() + CompletedGoal.UpdateDescription.ToString());
			//�ǲ�����������
			if (BSelectedInJournal())
			{	
				QuestManager->MainUserWidget->QuestJournal->UpdateDescription();
			}

		}
		

		CurrentGoals.Remove(CompletedGoal);

		//��ȡ���
		int WidgetIndex = CurrentGoalIndices.Find(SubGoalIndex);
		//UI���Ƴ�
		QuestWidget->SubGoalWidgets[WidgetIndex]->RemoveFromParent();
		//��������� ���������Ƴ�
		CurrentHuntedAmounts.RemoveAt(WidgetIndex);

		//�������Ƴ�
		QuestWidget->SubGoalWidgets.RemoveAt(WidgetIndex);
		//�������Ƴ�
		CurrentGoalIndices.Remove(SubGoalIndex);
	
		if (bSuccess)
		{
			CompletedSubGoals.Add(FCopmletdeGoal{ SubGoalIndex,CompletedGoal,true });

			if (CompletedGoal.bFaillMeansQuestFaill)
			{	
				CurrentMustSuccedCount++;
			}

			
			//                     ��ǰ���������֮������������������     ������������������
			for (auto i : CompletedGoal.FollwingSubGoallIndices)
			{
				CurrentGoalIndices.Add(i);
				CurrentGoals.Add(QuestInfo.SubGoals[i]);

				if (i > CurrentHuntedAmounts.Num())
				{
					CurrentHuntedAmounts.Add(0);
				}

				USubGoalWidget* SubGoalWidget = CreateWidget<USubGoalWidget>(GetWorld(), LoadClass<USubGoalWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UI/Quest/WBP_SubQuest.WBP_SubQuest_C'")));
				SubGoalWidget->GoalInfo = QuestInfo.SubGoals[i];
				SubGoalWidget->AssignedQuest = this;
				SubGoalWidget->QuestWidget = QuestWidget;
				//��ӵ�����
				QuestWidget->SubGoalWidgets.Add(SubGoalWidget);
				QuestWidget->SubGoalBox->AddChild(SubGoalWidget);
			}

		}
		else
		{

			//������ʧ��
			CompletedSubGoals.Add(FCopmletdeGoal{ SubGoalIndex,CompletedGoal,false });

			if (CompletedGoal.bFaillMeansQuestFaill)
			{
				//����ʧ��  ������ʶ��=������ʧ��
				CurrentState=EQuestSates::FailedQuest;
				QuestManager->bCurrentQuestFinished=true;

				for(int i :CurrentGoalIndices)
				{
					CompletedSubGoals.Add(FCopmletdeGoal{ SubGoalIndex,CompletedGoal,false });
				}

				CurrentGoalIndices.Empty();
				CurrentHuntedAmounts.Empty();
				CurrentGoals.Empty();

				ListEntryWidget->RemoveFromParent();
				QuestWidget->RemoveFromParent();
				QuestManager->OnQuestEnd(this);
				
				return true;
			}
			else
			{
				
				
				for (auto i : CompletedGoal.FollwingSubGoallIndices)
				{
					CurrentGoalIndices.Add(i);
					CurrentGoals.Add(QuestInfo.SubGoals[i]);

					if (i > CurrentHuntedAmounts.Num())
					{
						CurrentHuntedAmounts.Add(0);
					}

					USubGoalWidget* SubGoalWidget = CreateWidget<USubGoalWidget>(GetWorld(), LoadClass<USubGoalWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UI/Quest/WBP_SubQuest.WBP_SubQuest_C'")));
					SubGoalWidget->GoalInfo = QuestInfo.SubGoals[i];
					SubGoalWidget->AssignedQuest = this;
					SubGoalWidget->QuestWidget = QuestWidget;
					//��ӵ�����
					QuestWidget->SubGoalWidgets.Add(SubGoalWidget);
					QuestWidget->SubGoalBox->AddChild(SubGoalWidget);
				}

				//���»�����һ������ĸ���
				if (CompletedGoal.bUpdateQuestDescription)
				{
					CurrentDescription = FText::FromString(CurrentDescription.ToString() + CompletedGoal.UpdateDescription.ToString());
					//�ǲ�����������
					if (BSelectedInJournal())
					{
						QuestManager->MainUserWidget->QuestJournal->UpdateDescription();
					}
				}

			}

		}

		//�������ǲ����Ѿ����
		if (CurrentGoalCount >= TotalGoalAmount)
		{	

			QuestManager->bCurrentQuestFinished = true;

		//�����������Ѿ�����һ��
			if (CurrentMustSuccedCount >= TotalMustSuccedAmount)
			{	//���Ա�����ɵ��������
				CurrentState=EQuestSates::CompletedQuest;
			} 
			else
			{	
				CurrentState=EQuestSates::FailedQuest;
			}

			CurrentGoalIndices.Empty();
			CurrentHuntedAmounts.Empty();
			CurrentGoals.Empty();

			ListEntryWidget->RemoveFromParent();
			QuestWidget->RemoveFromParent();
			QuestManager->OnQuestEnd(this);

			return true;

		}
		









		if (BSelectedInJournal())
		{
			QuestManager->MainUserWidget->QuestJournal->GenerateSubGoal();
		}

		//���Ҫ��ɵ�������ѡ������� ����ѡ�ŵ�������ĵ�һ��
		if (SubGoalIndex==SelectedSubgGoalIndex && QuestWidget->SubGoalWidgets.Num()>0)
		{
			QuestWidget->SelectSubgGoal(QuestWidget->SubGoalWidgets[0]);
		}


		return true;
	} 
	else
	{
		return false;
	}
}




bool ABaseQuest::GoToNextSubGoal()
{
	if (CurrentGoalIndices.Max()+1 <=QuestInfo.SubGoals.Num())
	{
		int NextIndex = CurrentGoalIndices.Max() + 1;
		CurrentGoalIndices.Empty();
		CurrentGoalIndices.Add(NextIndex);

		UpdateSubGoal();
		return true;
	} 
	else
	{
		return false;
	}
}

bool ABaseQuest::BSelectedInJournal()
{

	return (QuestManager->MainUserWidget->QuestJournal->SelectedQuest && QuestManager->MainUserWidget->QuestJournal->SelectedQuest == this);
}


void ABaseQuest::CaculateGoalAmount()
{
	for (FGoalInfo GoalInfo:QuestInfo.SubGoals)
	{
		TotalGoalAmount++;
		if (GoalInfo.bFaillMeansQuestFaill)
		{
			TotalMustSuccedAmount++;
		}
	}
}


// Called every frame
void ABaseQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

