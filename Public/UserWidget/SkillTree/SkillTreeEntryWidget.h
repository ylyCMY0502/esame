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

	//Upgradebox�벻һ��
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

	//�������ĸ�����
	class USubTreeWidget* SubTree;
	//���������һ������
	ABaseSkill* SkillActor;
	//���ɵ��ࣿ???
	TSubclassOf<ABaseSkill> SkillClass;
	//�����ǲ�ѧϰ��
	bool bSpellLearnt;



	//���ؼ��ܵ����ȼ�
	int GetAmountOfStages();
	//��������
	void UpdateStagesText();
	//���¼���ͼƬ
	void UpdataIcon();
	
	//UpgradeBox 
	void UpdataUpgradeOverlay();
	//����ͼ������
	void OnSpellLearnt();

	//��ť���
	UFUNCTION()
	void OnPlusButtonClicked();
	UFUNCTION()
	void OnMinusButtonClicked();


	//��д ֻ������Ҽ����µ�ʱ�򣬲���ִ�м��ķ���
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//����鵽��ק �ǵ���
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;


};
