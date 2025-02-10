// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quest/QuestEnum.h"
#include "QuestCategoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuestCategoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock*CategoryNames;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExpandButton;

	

	UPROPERTY(meta = (BindWidget))
	class UImage* FoldImage;

protected:
	UPROPERTY(EditAnywhere)
	EQuestSates CurrentState=EQuestSates::CurrentQuest;

	bool bExpand=false;

	virtual void NativeConstruct()override;

	UFUNCTION()
	void OnExpandButtonClick();

public:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* QuestBox;

	void SetCategoryNames(FText CategoryNamesText);
	
	


};
