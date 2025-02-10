// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Skill/SkillTreeStruct.h"
#include "MainTreeWidget.generated.h"


class UTextBlock;
/**
 * ���ܵ�������
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

	//���ܴ����齨
	class USkillTreeComponent* SkillTreeCom;
	//��ǰѡ���Category ��Ӧ�����
	int CurrentlySelectedIndex = -1;
	//��ӦSkill Buff
	TArray<class UCategoryWidget*>CategoryWidgets;
	//CategoryWidget����Ϣ
	TArray<FSubTreeCategory>TreeCategories;
	//
	TArray<class USubTreeWidget*>SubTreeWidgets;

	UFUNCTION()
	void OnCategoryClicked(int Index);
	
	//�رռ��ܴ��ڵ�
	UFUNCTION()
	void OnCloseButtonClicked();

	//���µ����ȼ���
	void UpdateLevel();
	void UpdateSp();

	//���ɼ��ܽ����밴ť��
	void GenerateCategories();

	//��ʼ��
	void Initialize(class USkillTreeComponent* AssignedTreeComp);
};
