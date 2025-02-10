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
struct FSubTreeConnection  //����֮�������
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Skill")
	FVector2D Position;
	UPROPERTY(EditAnywhere, Category = "Skill")
	FVector2D Size;
	UPROPERTY(EditAnywhere, Category = "Skill")
	FWidgetTransform Transfrom;

	//�����ߵ�ǰ�ü���
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ABaseSkill>ForSpell;
};



USTRUCT()
struct FSubTreeEntry //����ͼ��+- 0/5   USkillTreeEntryWidget
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Skill")
	FVector2D Position;

	//Entry��Ӧ�ļ���
	UPROPERTY(EditAnywhere,Category="Skill")
	TSubclassOf<class ABaseSkill>SpellClass; 

};



USTRUCT()
struct FSubTreeContent  //WBP_SubTree��ż��������ߵ�

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

	//��ť��Ӧ��ue�༭���ڵ�WBP_SubTree
	UPROPERTY(EditAnywhere, Category = "Skill")
	FSubTreeContent Content;
};

//****************

UCLASS()
class RPGPROJECT_API USkillTreeStruct : public UObject
{
	GENERATED_BODY()
	
};
