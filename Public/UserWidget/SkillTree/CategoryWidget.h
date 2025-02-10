// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(disable: 4458)

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CategoryWidget.generated.h"

/**
 *  �������л���ť
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

	//��ʾBuff Skill
	FText CategoryName;
	
	//�ж��ǲ��Ǳ�ѡ��
	bool bSelected;

	//��Ӧ������
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
