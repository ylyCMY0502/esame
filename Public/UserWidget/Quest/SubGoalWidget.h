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

	//那个任务
	class ABaseQuest* AssignedQuest;
	//那个UI
	class UQuestWidget* QuestWidget;
	
	//序列号
	int GoalIndex;

	//击杀怪物的数量索引
	int HuntIndex;
	
	void Update();

	virtual void NativeConstruct()override;


	UFUNCTION()
	void OnSelectButtonClicked();
};
