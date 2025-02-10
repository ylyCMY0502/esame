// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestEnum.generated.h"

/**
 * 
 */

UENUM()
enum class EQuestCategories
{
	//任务类型
	MainQuest,//主线任务
	SideQuest,//支线任务
	Events//事件
};

UENUM()
enum class EGoalTypes
{
	//任务目标类型
	//该NPC没有任务
	Custom, 
	//刷怪任务
	Hunt,
	//搜索任务
	Find,
	//对话任务
	Talk 
};


UENUM()
enum class ERegions
{
	//任务的区域
	Village UMETA(DisplayName="Village"),
	CastleRuins UMETA(DisplayName = "CastleRuins")
};


UENUM()
enum class EQuestSates
{
	//当前任务
	CurrentQuest,
	//完成任务
	CompletedQuest,
	//失败任务
	FailedQuest
};


UENUM()
enum class EGoalState
{
	//当前任务
	Current,
	//完成任务
	Completed,
	//失败任务
	Failed
};



UCLASS()
class RPGPROJECT_API UQuestEnum : public UObject
{
	GENERATED_BODY()
	
};
