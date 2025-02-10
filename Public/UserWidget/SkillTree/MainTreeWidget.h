// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Skill/SkillTreeStruct.h"
#include "MainTreeWidget.generated.h"


class UTextBlock;
/**
 * 技能的主界面
 */
UCLASS()
class RPGPROJECT_API UMainTreeWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct()override;
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SPText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelText;
	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* SubTreeSwitcher;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* CategoriesBox;

	//技能窗口组建
	class USkillTreeComponent* SkillTreeCom;
	//当前选择的Category 对应的序号
	int CurrentlySelectedIndex = -1;
	//对应Skill Buff
	TArray<class UCategoryWidget*>CategoryWidgets;
	//CategoryWidget的信息
	TArray<FSubTreeCategory>TreeCategories;
	//
	TArray<class USubTreeWidget*>SubTreeWidgets;

	UFUNCTION()
	void OnCategoryClicked(int Index);
	
	//关闭技能窗口的
	UFUNCTION()
	void OnCloseButtonClicked();

	//更新点数等级的
	void UpdateLevel();
	void UpdateSp();

	//生成技能界面与按钮的
	void GenerateCategories();

	//初始化
	void Initialize(class USkillTreeComponent* AssignedTreeComp);
};
