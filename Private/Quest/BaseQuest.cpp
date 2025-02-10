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


//用于更新 当前任务的数组
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
	//设置数组的长度
	CurrentHuntedAmounts.SetNum(CurrentGoalIndices.Num());
}



bool ABaseQuest::OnSubGoalCompleted(int SubGoalIndex,bool bSuccess)
{
	//
	if (CurrentGoalIndices.Contains(SubGoalIndex))
	{
		CurrentGoalCount++;

		//获取索引对应的任务
		FGoalInfo CompletedGoal = QuestInfo.SubGoals[SubGoalIndex];
		

		//更新会有下一个任务的更新
		if(CompletedGoal.bUpdateQuestDescription  && bSuccess)
		{
			CurrentDescription=FText::FromString(CurrentDescription.ToString() + CompletedGoal.UpdateDescription.ToString());
			//是不是主界面上
			if (BSelectedInJournal())
			{	
				QuestManager->MainUserWidget->QuestJournal->UpdateDescription();
			}

		}
		

		CurrentGoals.Remove(CompletedGoal);

		//获取序号
		int WidgetIndex = CurrentGoalIndices.Find(SubGoalIndex);
		//UI上移除
		QuestWidget->SubGoalWidgets[WidgetIndex]->RemoveFromParent();
		//怪物的数量 从数组中移除
		CurrentHuntedAmounts.RemoveAt(WidgetIndex);

		//数组中移除
		QuestWidget->SubGoalWidgets.RemoveAt(WidgetIndex);
		//索引中移除
		CurrentGoalIndices.Remove(SubGoalIndex);
	
		if (bSuccess)
		{
			CompletedSubGoals.Add(FCopmletdeGoal{ SubGoalIndex,CompletedGoal,true });

			if (CompletedGoal.bFaillMeansQuestFaill)
			{	
				CurrentMustSuccedCount++;
			}

			
			//                     当前子任务完成之后接下来子任务的索引     创建接下来的子任务
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
				//添加到数组
				QuestWidget->SubGoalWidgets.Add(SubGoalWidget);
				QuestWidget->SubGoalBox->AddChild(SubGoalWidget);
			}

		}
		else
		{

			//子任务失败
			CompletedSubGoals.Add(FCopmletdeGoal{ SubGoalIndex,CompletedGoal,false });

			if (CompletedGoal.bFaillMeansQuestFaill)
			{
				//任务失败  子任务识别=总任务失败
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
					//添加到数组
					QuestWidget->SubGoalWidgets.Add(SubGoalWidget);
					QuestWidget->SubGoalBox->AddChild(SubGoalWidget);
				}

				//更新会有下一个任务的更新
				if (CompletedGoal.bUpdateQuestDescription)
				{
					CurrentDescription = FText::FromString(CurrentDescription.ToString() + CompletedGoal.UpdateDescription.ToString());
					//是不是主界面上
					if (BSelectedInJournal())
					{
						QuestManager->MainUserWidget->QuestJournal->UpdateDescription();
					}
				}

			}

		}

		//总任务是不是已经完成
		if (CurrentGoalCount >= TotalGoalAmount)
		{	

			QuestManager->bCurrentQuestFinished = true;

		//所以子任务都已经过了一边
			if (CurrentMustSuccedCount >= TotalMustSuccedAmount)
			{	//所以必须完成的任务都完成
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

		//如果要完成的任务是选择的任务 将会选着到子任务的第一个
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

