// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/QuestManager.h"
#include "Quest/BaseQuest.h"
#include "UserWidget/Quest/SubGoalWidget.h"
#include "UserWidget/Quest/QuestWidget.h"
#include "UserWidget/MainUserWidget.h"
#include "Components/TextBlock.h"
#include "Quest/GoalActor.h"
#include "Character/RBaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "UserWidget/MiniMapWidget.h"
#include "UserWidget/Quest/QuestJournalWidget.h"
#include "Enemy/NormalEnemy.h"
#include "NPC/BsaeNPC.h"
#include "UMG/Public/Components/Overlay.h"
#include "UMG/Public/Components/Image.h"

// Sets default values
AQuestManager::AQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void AQuestManager::BeginPlay()
{
	Super::BeginPlay();
	
}


//
float AQuestManager::GetDistanceToGoal()
{
	if(PlayerCharacter != nullptr && CurrentGoal != nullptr)
	//ARBaseCharacter			//AGoalActor
	return FMath::FloorToInt(FVector::Dist2D(PlayerCharacter->GetActorLocation(), CurrentGoal->GetActorLocation()));
	else
	{
		return 0;
	}
}


void AQuestManager::UpdateDrectionArrow()
{
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(), CurrentGoal->GetActorLocation());
	//UMiniMapWidget
	MainUserWidget->Minimap->RotateDirection(Rotator.Yaw);
}

void AQuestManager::OnSwitchSubQuest()
{
	if (CurrentGoal)
	{
		CurrentGoal->Destroy();
	}
	CurrentSubGoal = CurrentQuest->QuestInfo.SubGoals[CurrentQuest->SelectedSubgGoalIndex];

	if (CurrentSubGoal.GoalLocation.bHasLocation)
	{
		CurrentGoal = GetWorld()->SpawnActor<AGoalActor>(GoalActorClass, CurrentSubGoal.GoalLocation.Location, FRotator::ZeroRotator);
		//更新初始化的技能
		CurrentGoal->InitializeMinimapIcon(CurrentSubGoal.bUseRadius, CurrentSubGoal.Radius, CurrentSubGoal.CircleColor);


		MainUserWidget->Minimap->SetDistanceText(GetDistanceToGoal());
		UpdateDrectionArrow();

		if (GetDistanceToGoal() > ShowHintDistance)
		{
			MainUserWidget->Minimap->SetGoalHitVisible(true);
		} 
		else
		{
			MainUserWidget->Minimap->SetGoalHitVisible(false);
		}
	}
	else
	{
		CurrentGoal = nullptr;
		MainUserWidget->Minimap->SetGoalHitVisible(false);
	}


}


//在主角中调用
void AQuestManager::OnPlayerMove()
{
	if (CurrentGoal)
	{
		MainUserWidget->Minimap->SetDistanceText(GetDistanceToGoal());
		UpdateDrectionArrow();
		if (GetDistanceToGoal() > ShowHintDistance)
		{
			MainUserWidget->Minimap->SetGoalHitVisible(true);
		}
		else
		{
			MainUserWidget->Minimap->SetGoalHitVisible(false);
		}
	}
}

void AQuestManager::SelectNewQuest(class ABaseQuest* NewQuest, class USubGoalWidget* SubGoalWidget)
{
	if (CurrentQuest)
	{
		//UTextBlock
		CurrentQuest->QuestWidget->QuestName->SetIsEnabled(false);
		//------------------
		//class USubGoalWidget* SelectedSubgGoalWidget; //当前选择要完成的任务
		CurrentQuest->QuestWidget->SelectSubgGoal(nullptr);
		//------------------------------
	}
	
	CurrentQuest = NewQuest;
	CurrentQuest->QuestWidget->OnQuestSelect(SubGoalWidget);
}

bool AQuestManager::AddNewQuset(TSubclassOf<ABaseQuest> QuestClass)
{
	if (QuestClass && !AllQuestClasses.Contains(QuestClass))
	{
		AllQuestClasses.Add(QuestClass);

		ABaseQuest* TempQuest = GetWorld()->SpawnActor<ABaseQuest>(QuestClass, FVector::ZeroVector, FRotator::ZeroRotator);
		CurrentQuestActors.Add(TempQuest);
		TempQuest->QuestManager=this;


		TempQuest->SetupStartinGoals();
	
		//UMainUserWidget
		TempQuest->QuestWidget = MainUserWidget->AddQuestToList(TempQuest);
		TempQuest->QuestWidget->QuestManager = this;
		TempQuest->QuestWidget->UpdataQuest();

		MainUserWidget->QuestJournal->AddQuestEntry(TempQuest);

		if (CurrentQuestActors.Num()<=1)
		{
			SelectNewQuest(TempQuest, TempQuest->QuestWidget->SubGoalWidgets[0]);
		}

		PlayerCharacter->PlayAddNewQuestAnimation();

		return true;
	}
	else
	{
		return false;
	}
}

// Called every frame
void AQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQuestManager::OnEnemyKilled(TSubclassOf<class ANormalEnemy>Enemy)
{
	//for (ABaseQuest* CurrentQuest : CurrentQuestActors)
	for(int j=0; j< CurrentQuestActors.Num();j++)
	{	
		for (int i=0;i< CurrentQuestActors[j]->CurrentGoals.Num(); i++)
		{
			if (CurrentQuestActors[j]->CurrentGoals[i].Type==EGoalTypes::Hunt  && CurrentQuestActors[j]->CurrentGoals[i].GoalClass==Enemy)//怪物
			{
				CurrentQuestActors[j]->CurrentHuntedAmounts[i]++;
				//是不是击杀敌人完成
				if (CurrentQuestActors[j]->CurrentHuntedAmounts[i] >= CurrentQuestActors[j]->CurrentGoals[i].AmountToHunt)
				{
					CurrentQuestActors[j]->OnSubGoalCompleted(CurrentQuestActors[j]->CurrentGoalIndices[i],true);

				}

				if (bCurrentQuestFinished)
				{
					bCurrentQuestFinished = false;
					break;
				}


				CurrentQuestActors[j]->QuestWidget->SubGoalWidgets[i]->Update();

				if (CurrentQuestActors[j]->BSelectedInJournal())
				{
					MainUserWidget->QuestJournal->GenerateSubGoal();
				}
	
			}
		}
	}
}


void AQuestManager::OnObjectFound(TSubclassOf<class ABaseQuestProps>Props)
{
	//for (ABaseQuest* CurrentQuest : CurrentQuestActors)

	for (int j = 0; j < CurrentQuestActors.Num(); j++)
	{
		for (int i = 0; i < CurrentQuestActors[j]->CurrentGoals.Num(); i++)
		{
			if (CurrentQuestActors[j]->CurrentGoals[i].Type == EGoalTypes::Find && CurrentQuestActors[j]->CurrentGoals[i].GoalClass == Props)//物品
			{
				CurrentQuestActors[j]->OnSubGoalCompleted(CurrentQuestActors[j]->CurrentGoalIndices[i],true);

				if (bCurrentQuestFinished)
				{
					bCurrentQuestFinished=false;
					break;
				}
			}
		}
	}
}

void AQuestManager::OnTalkToNPC(TSubclassOf<class ABsaeNPC> NPC, int NPCId)
{

	//for (ABaseQuest* CurrentQuest : CurrentQuestActors)
	for (int j = 0; j < CurrentQuestActors.Num(); j++)
	{
		for (int i = 0; i < CurrentQuestActors[j]->CurrentGoals.Num(); i++)
		{
			if (CurrentQuestActors[j]->CurrentGoals[i].Type == EGoalTypes::Talk && CurrentQuestActors[j]->CurrentGoals[i].GoalClass == NPC && CurrentQuestActors[j]->CurrentGoals[i].GoalId== NPCId)//物品
			{
				CurrentQuestActors[j]->OnSubGoalCompleted(CurrentQuestActors[j]->CurrentGoalIndices[i],true);


				if (bCurrentQuestFinished)
				{
					bCurrentQuestFinished = false;
					break;
				}
			}
		}
	}

}



void AQuestManager::OnQuestEnd(ABaseQuest* Quest)
{
	CurrentQuestActors.Remove(Quest);

	switch(Quest->CurrentState)
	{
		case EQuestSates::CompletedQuest:
			CompletedQuestActors.Add(Quest);
			break;

		case EQuestSates::FailedQuest:
			FailedQuestActors.Add(Quest);
			break;

		default:
			break;
	}

	MainUserWidget->QuestJournal->AddQuestEntry(Quest);

	if(CurrentQuest==Quest)
	{
		CurrentQuest=nullptr;

		if(CurrentGoal)
		{
			CurrentGoal->Destroy();
		}
		


		MainUserWidget->Minimap->SetGoalHitVisible(false);
		/*MainUserWidget->Minimap->DistanceOverlay->SetVisibility(ESlateVisibility::Hidden);
		MainUserWidget->Minimap->DirectionArrow->SetVisibility(ESlateVisibility::Hidden);*/
	

		if (CurrentQuestActors.Num()>0)
		{
		
		if (CurrentQuestActors[0])
		{
			SelectNewQuest(CurrentQuestActors[0],CurrentQuestActors[0]->QuestWidget->SubGoalWidgets[0]);
		}

		}
	}


	if(Quest->BSelectedInJournal())
	{
		MainUserWidget->QuestJournal->SelectedQuest=nullptr;

		MainUserWidget->QuestJournal->OnQuestClicked(nullptr);
	}


	if (Quest->CurrentState ==EQuestSates::CompletedQuest)
	{
		PlayerCharacter->IncreaseChangeCurrentExp(Quest->QuestInfo.CompletionReward.Experience);
	}
}