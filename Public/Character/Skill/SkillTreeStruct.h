// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Slate/WidgetTransform.h"
#include "SkillTreeStruct.generated.h"

/**
 * 
 */



USTRUCT()
struct FSubTreeConnection  //技能之间的连线
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Skill")
	FVector2D Position;
	UPROPERTY(EditAnywhere, Category = "Skill")
	FVector2D Size;
	UPROPERTY(EditAnywhere, Category = "Skill")
	FWidgetTransform Transfrom;

	//连接线的前置技能
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ABaseSkill>ForSpell;
};



USTRUCT()
struct FSubTreeEntry //技能图标+- 0/5   USkillTreeEntryWidget
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Skill")
	FVector2D Position;

	//Entry对应的技能
	UPROPERTY(EditAnywhere,Category="Skill")
	TSubclassOf<class ABaseSkill>SpellClass; 

};



USTRUCT()
struct FSubTreeContent  //WBP_SubTree存放技能与连线的

{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FSubTreeEntry>Skills;
	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FSubTreeConnection>Connections;
};


//WBP_Category
USTRUCT()
struct FSubTreeCategory
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Skill")
	FText Name;

	//按钮对应的ue编辑器内的WBP_SubTree
	UPROPERTY(EditAnywhere, Category = "Skill")
	FSubTreeContent Content;
};

//****************

UCLASS()
class RPGPROJECT_API USkillTreeStruct : public UObject
{
	GENERATED_BODY()
	
};
