// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quest/QuestEnum.h"
#include "QuestListEntryWidget.generated.h"

class UTextBlock;

UCLASS()
class RPGPROJECT_API UQuestListEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UButton*QuestButton;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuestNmae;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RegionName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SuggestedLevel;


public:
	
	class UQuestJournalWidget* Journal;
	class ABaseQuest* AssignedQuest;



	virtual void NativeConstruct()override;
	void UpdateLevelColor();
	void Update();
	FString GetERegionsEnumString(ERegions Region);

	void SetQuestNmae(FText QuestNmaeText);
	void SetRegionName(FText RegionNameText);
	void SetSuggestedLevel(FText SuggestedLevelText);

	UFUNCTION()
	void OnQuestButtonClickde();
};
