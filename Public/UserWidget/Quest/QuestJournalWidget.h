// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestJournalWidget.generated.h"

class UQuestCategoryWidget;
class UTextBlock;
class UVerticalBox;
class UButton;
class UHorizontalBox;

UCLASS()
class RPGPROJECT_API UQuestJournalWidget : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION()
	void OnCloseButtonClicke();
	
	
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox*QuestList;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* QuestDetailsBox;


	UPROPERTY(meta = (BindWidget))
	UQuestCategoryWidget* CatCurrent;

	UPROPERTY(meta = (BindWidget))
	UQuestCategoryWidget* CatCompleted;

	UPROPERTY(meta = (BindWidget))
	UQuestCategoryWidget* CatFailed;



	UPROPERTY(meta = (BindWidget))
	UTextBlock*QuestName;

	//����Main
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuestCategory;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuestRegion;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SuggestedLevel;

	//������
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* RewardBox;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox*EXPRewarlBox;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* GoldRewarlBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*EXPReward;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*GoldReward;
	//--------------
	//��������
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Description;
	
	//���������
	UPROPERTY(meta = (BindWidget))
	UVerticalBox*QuestGoalBox;




	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ButtonBox;

	UPROPERTY(meta = (BindWidget))
	UButton*SelectButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

public:
	class ARBaseCharacter*PlayerCharacter;

	class ABaseQuest*SelectedQuest;
	class AQuestManager* QuestManager;
	//class UQuestListEntryWidget* CurrentQuestListEntry;

	TArray<class UQuestListEntryWidget*>AllQuestListEntries;
	class UQuestListEntryWidget* CurrentUQuestListEntryWidget;

	void CloseQuestWindow();


	virtual void NativeConstruct()override;


	//���µȼ�
	void UpdateSuggestedLevelColor();
	//������������
	void UpdateDescription();
	//������UI
	void GenerateSubGoal();

	//�������񴰿�
	void UpdateDetailWindow();
	void Initialize(class AQuestManager* QuestManager);

	void OnQuestClicked(class UQuestListEntryWidget* ClickedQuestListEntryWidget);

	//��ӵ�ǰ���� �� ������� ʧ�������������UI
	void AddQuestEntry(class ABaseQuest* Quest);
	
	UFUNCTION()
	void OnSelectButyonClicked();

};
