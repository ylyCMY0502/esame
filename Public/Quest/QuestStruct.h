// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestEnum.h"
#include "QuestStruct.generated.h"



//任务完成后的奖励
USTRUCT()
struct FQuestReward
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,Category="QuestInfo")
	int Experience;
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	int Coin;
};

USTRUCT()
struct FGoalLocation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	bool bHasLocation;//任务是不是有位置
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	FVector Location;//任务的位置
};

USTRUCT()
struct  FGoalInfo    //任务目标信息
{
	GENERATED_USTRUCT_BODY()

	//任务类型
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	EGoalTypes Type = EGoalTypes::Custom;

	//T.ure 是没有任务
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	bool bCustomGoal;

	//任务的描述
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	FText GoalText;
	//任务的目标的名字
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	FText AdditionalName; 
	//要杀死的怪物的数量
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	int AmountToHunt;
	//目标怪物的位置
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	FGoalLocation GoalLocation;
	//当前任务完成，会有下一个任务的更新
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	bool bUpdateQuestDescription;
	
	//任务的描述
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	FText UpdateDescription; 

	//当前子任务完成之后接下来子任务的索引
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	TArray<int>FollwingSubGoallIndices;

	//是不是范围任务
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	bool bUseRadius; 
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	float Radius;
	//圈颜色
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	FLinearColor CircleColor;

	//任务类型
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	TSubclassOf<AActor>GoalClass;
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	int GoalId;

	//必须完成的任务
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	bool bFaillMeansQuestFaill;

	bool operator==(const FGoalInfo& GoalInfo)
	{
		return
		Type == GoalInfo.Type &&
		bCustomGoal== GoalInfo.bCustomGoal &&
		GoalInfo.GoalText.EqualTo(GoalText) &&
		GoalInfo.AdditionalName.EqualTo(AdditionalName)&&
		AmountToHunt== GoalInfo.AmountToHunt &&
		GoalLocation.bHasLocation== GoalInfo.GoalLocation.bHasLocation &&
		GoalLocation.Location== GoalInfo.GoalLocation.Location &&
		bUpdateQuestDescription== GoalInfo.bUpdateQuestDescription &&
		UpdateDescription.EqualTo(GoalInfo.UpdateDescription)&&
		FollwingSubGoallIndices == GoalInfo.FollwingSubGoallIndices &&
		bUseRadius== GoalInfo.bUseRadius &&
		Radius == GoalInfo.Radius &&
		CircleColor == GoalInfo.CircleColor&&
		GoalClass== GoalInfo.GoalClass &&
		GoalId==GoalInfo.GoalId &&
		bFaillMeansQuestFaill==GoalInfo.bFaillMeansQuestFaill
		;

	}
};

USTRUCT()
struct FQuestInfo
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	FText Name; //任务的名字
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	EQuestCategories Category;//任务的类型
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	FText Description; //任务的描述
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	ERegions Region;//任务的位置
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	FQuestReward CompletionReward;//任务的奖励
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	int SuggestedLevel;//角色的等级
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	float Difficulty; //任务难度

	//子任务
	UPROPERTY(EditAnywhere, Category = "QuestInfo")
	TArray<FGoalInfo>SubGoals;
};

USTRUCT()
struct FCopmletdeGoal
{
	GENERATED_USTRUCT_BODY()

	int GoalIndex;
	FGoalInfo GoalInfo;
	//是否成功
	bool bSuccessful;
};

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestStruct : public UObject
{
	GENERATED_BODY()
	
};
