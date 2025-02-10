// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	class UTextBlock* QuestName;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* SubGoalBox;
	
	//������
	class AQuestManager* QuestManager;
	//���������
	class ABaseQuest* AssignedQuest;
	//���������
	TArray<class USubGoalWidget*>SubGoalWidgets;  //С������


	//��ǰѡ��Ҫ��ɵ�����
	class USubGoalWidget* SelectedSubgGoalWidget;

	//������UI
	void GenerateSubWidgets();

	//��������
	void UpdataQuest();

	//�ж��ǲ��ǹ������ϵ�����
	bool BCurrnetQuest();


	//������ѡ��
	void  SelectSubgGoal(class USubGoalWidget* ClickeSubgGoalWidget);

	void OnQuestSelect(class USubGoalWidget* SubgGoalWidget);


};
