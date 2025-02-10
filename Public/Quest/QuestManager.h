// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestStruct.h"
#include "QuestManager.generated.h"

class ABaseQuest;
class AGoalActor;

UCLASS()
class RPGPROJECT_API AQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestManager();


	
	class ARBaseCharacter* PlayerCharacter;

	class UMainUserWidget* MainUserWidget;
	//当前任务
	ABaseQuest* CurrentQuest;

	//所以的任务类
	TArray<TSubclassOf<ABaseQuest>>AllQuestClasses;
	//任务的实例
	TArray<ABaseQuest*>CurrentQuestActors;
	TArray<ABaseQuest*>CompletedQuestActors;
	TArray<ABaseQuest*>FailedQuestActors;




	FGoalInfo CurrentSubGoal;

	AGoalActor* CurrentGoal;

	UPROPERTY(EditAnywhere,Category="Quset")
	TSubclassOf<AGoalActor>GoalActorClass;
	

	float GetDistanceToGoal();//两个的距离
	//箭头转向
	void UpdateDrectionArrow();
	//更新子技能显示
	void OnSwitchSubQuest();
	//更行移动
	void OnPlayerMove();

protected: 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category="Quset")
	float ShowHintDistance;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SelectNewQuest(ABaseQuest* NewQuest, class USubGoalWidget* SubGoalWidget);

	//添加所以的任务类
	UFUNCTION(BlueprintCallable)
	bool AddNewQuset(TSubclassOf<class ABaseQuest> QuestClass);
	//敌人（怪物） 死亡后
	void OnEnemyKilled(TSubclassOf<class ANormalEnemy>Enemy);

	//与物品交互后
	void OnObjectFound(TSubclassOf<class ABaseQuestProps>Props);

	void OnTalkToNPC(TSubclassOf<class ABsaeNPC> NPC, int NPCId);
	//完成任务后
	void OnQuestEnd(ABaseQuest*Quest);

	bool bCurrentQuestFinished;

};
