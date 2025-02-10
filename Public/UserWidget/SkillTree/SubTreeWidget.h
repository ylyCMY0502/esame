// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Skill/SkillTreeStruct.h"
#include "SubTreeWidget.generated.h"

/**
 * 放技能连线 与 技能的
 */
UCLASS()
class RPGPROJECT_API USubTreeWidget : public UUserWidget
{
	GENERATED_BODY()


	virtual void NativeConstruct()override;

public:

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Canvas;

	//存放技能连线参数信息的
	FSubTreeContent Content;

	//放技能UI
	TArray<class USkillTreeEntryWidget*>SkillEntries;
	//放连线UI
	TArray<class UConnectionWidget*>Connections;
	
	class UMainTreeWidget* MainTree;


	void GenerateContent();
};
