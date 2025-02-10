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
	
	//管理器
	class AQuestManager* QuestManager;
	//分配的任务
	class ABaseQuest* AssignedQuest;
	//子任务界面
	TArray<class USubGoalWidget*>SubGoalWidgets;  //小点任务


	//当前选择要完成的任务
	class USubGoalWidget* SelectedSubgGoalWidget;

	//生成子UI
	void GenerateSubWidgets();

	//更新任务
	void UpdataQuest();

	//判断是不是管理器上的任务
	bool BCurrnetQuest();


	//子任务选着
	void  SelectSubgGoal(class USubGoalWidget* ClickeSubgGoalWidget);

	void OnQuestSelect(class USubGoalWidget* SubgGoalWidget);


};
