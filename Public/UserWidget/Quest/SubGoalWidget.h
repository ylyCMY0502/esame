// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quest/QuestStruct.h"
#include "SubGoalWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USubGoalWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UBorder* GoalBorder;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoalText;
	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton;



	FGoalInfo GoalInfo;

	//�Ǹ�����
	class ABaseQuest* AssignedQuest;
	//�Ǹ�UI
	class UQuestWidget* QuestWidget;
	
	//���к�
	int GoalIndex;

	//��ɱ�������������
	int HuntIndex;
	
	void Update();

	virtual void NativeConstruct()override;


	UFUNCTION()
	void OnSelectButtonClicked();
};
