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
	//��ǰ����
	ABaseQuest* CurrentQuest;

	//���Ե�������
	TArray<TSubclassOf<ABaseQuest>>AllQuestClasses;
	//�����ʵ��
	TArray<ABaseQuest*>CurrentQuestActors;
	TArray<ABaseQuest*>CompletedQuestActors;
	TArray<ABaseQuest*>FailedQuestActors;




	FGoalInfo CurrentSubGoal;

	AGoalActor* CurrentGoal;

	UPROPERTY(EditAnywhere,Category="Quset")
	TSubclassOf<AGoalActor>GoalActorClass;
	

	float GetDistanceToGoal();//�����ľ���
	//��ͷת��
	void UpdateDrectionArrow();
	//�����Ӽ�����ʾ
	void OnSwitchSubQuest();
	//�����ƶ�
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

	//������Ե�������
	UFUNCTION(BlueprintCallable)
	bool AddNewQuset(TSubclassOf<class ABaseQuest> QuestClass);
	//���ˣ���� ������
	void OnEnemyKilled(TSubclassOf<class ANormalEnemy>Enemy);

	//����Ʒ������
	void OnObjectFound(TSubclassOf<class ABaseQuestProps>Props);

	void OnTalkToNPC(TSubclassOf<class ABsaeNPC> NPC, int NPCId);
	//��������
	void OnQuestEnd(ABaseQuest*Quest);

	bool bCurrentQuestFinished;

};
