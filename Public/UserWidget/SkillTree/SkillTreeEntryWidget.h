// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeEntryWidget.generated.h"

class UButton;
class UTextBlock;
class UOverlay;

class ABaseSkill;

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USkillTreeEntryWidget : public UUserWidget
{
	GENERATED_BODY()


	virtual void NativeConstruct()override;


protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDragDropOperation>SkillDragOperation;
public:
	UPROPERTY(meta = (BindWidget))
	UOverlay* SkillOverlay;
	UPROPERTY(meta = (BindWidget))
	class UImage* SkillIcon;

	//Upgradebox与不一样
	UPROPERTY(meta = (BindWidget))
	UOverlay* UpgradeOverlay;
	UPROPERTY(meta = (BindWidget))
	UButton* PlusButton;
	UPROPERTY(meta = (BindWidget))
	UButton* MinusButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrnetStageText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StageText;

	//生成在哪个窗口
	class USubTreeWidget* SubTree;
	//保存的是那一个技能
	ABaseSkill* SkillActor;
	//生成的类？???
	TSubclassOf<ABaseSkill> SkillClass;
	//技能是不学习过
	bool bSpellLearnt;



	//返回技能的最大等级
	int GetAmountOfStages();
	//更新文字
	void UpdateStagesText();
	//更新技能图片
	void UpdataIcon();
	
	//UpgradeBox 
	void UpdataUpgradeOverlay();
	//更新图标明暗
	void OnSpellLearnt();

	//按钮点击
	UFUNCTION()
	void OnPlusButtonClicked();
	UFUNCTION()
	void OnMinusButtonClicked();


	//重写 只有鼠标右键按下的时候，才能执行检测的方法
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//当检查到拖拽 是调用
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;


};
