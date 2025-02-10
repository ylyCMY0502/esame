// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestStruct.h"
#include "BaseQuest.generated.h"

UCLASS()
class RPGPROJECT_API ABaseQuest : public AActor
{
	GENERATED_BODY()
	

	// Sets default values for this actor's properties
	ABaseQuest();

public:
	UPROPERTY(EditAnywhere,Category="QuestInfomation")
	FQuestInfo QuestInfo;

	//初始的子任务的索引，初始的时候，可能有一个也可能有多少个
	UPROPERTY(EditAnywhere, Category = "QuestInfomation")
	TArray<int> StartingSubGoalIndices = { 0 };

	//当前子任务的索引
	TArray<int> CurrentGoalIndices;

	//当前所有的任务；
	TArray<FGoalInfo> CurrentGoals;

	//已经完成的任务
	UPROPERTY(VisibleAnywhere, Category = "QuestInfomation")
	TArray<FCopmletdeGoal>CompletedSubGoals;


	//当前选择的要完成的任务的索引
	int SelectedSubgGoalIndex;


	//任务的状态
	EQuestSates CurrentState;
	//任务的描述
	FText CurrentDescription;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void UpdateSubGoal();

	//添加新的任务
	bool GoToNextSubGoal();

public:	
	//总的
	int TotalGoalAmount;
	//完成
	int CurrentGoalCount;
	int TotalMustSuccedAmount;
	//必须完成
	int CurrentMustSuccedCount;

	




	//当前击杀的怪物数量
	TArray<int> CurrentHuntedAmounts;
	//当期任务的UI
	class UQuestWidget* QuestWidget;

	class UQuestListEntryWidget* ListEntryWidget;

	class AQuestManager* QuestManager;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//设置初始化
	void SetupStartinGoals();

	//完成的子任务   //是不是成功过完成
	bool OnSubGoalCompleted(int SubGoalIndex, bool bSuccess);

	//是不是UI 上的选着 是不是自己
	bool BSelectedInJournal();

	FORCEINLINE  FGoalInfo GoalAtIndex(int Index){return QuestInfo.SubGoals[Index];}
	
	void CaculateGoalAmount();

};
