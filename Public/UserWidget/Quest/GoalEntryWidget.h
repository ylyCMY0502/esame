// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quest/QuestStruct.h"
#include "GoalEntryWidget.generated.h"


class UTexture2D;

UCLASS()
class RPGPROJECT_API UGoalEntryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* GoalText;

	UPROPERTY(meta=(BindWidget))
	class UImage* GoalStateImage;

	UPROPERTY(EditAnywhere,Category="SateIcon")
	UTexture2D* CurrentUTexture;
	UPROPERTY(EditAnywhere, Category = "SateIcon")
	UTexture2D* SuccessUTexture;
	UPROPERTY(EditAnywhere, Category = "SateIcon")
	UTexture2D* FailedUTexture;
	
public:


	FGoalInfo GoalInfo;

	EGoalState GoalState;

	class UQuestJournalWidget* Journal;

	void Update();

	virtual void NativeConstruct()override;

	void SetGoalText(FText Text);

	int HuntIndex;
};
