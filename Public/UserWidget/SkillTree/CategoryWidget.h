// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(disable: 4458)

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CategoryWidget.generated.h"

/**
 *  技能树切换按钮
 */
UCLASS()
class RPGPROJECT_API UCategoryWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* CategoryButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CategoryNameText;

	//显示Buff Skill
	FText CategoryName;
	
	//判断是不是被选择
	bool bSelected;

	//对应的索引
	int Index;

	class UMainTreeWidget* MainTreeRef;

	UFUNCTION()
	void OnCategoryButtonHoverd();
	UFUNCTION()
	void OnCategoryButtonUnhoverd();

	UFUNCTION()
	void OnCategoryButtonClicked();

	void ChangeState(bool bSelected);

};
