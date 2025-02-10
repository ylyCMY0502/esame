// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillHotkey.h"
#include "Containers/Queue.h"
#include "InventorySystem/InventoryStruct.h"
#include "MainUserWidget.generated.h"

/**
 * 
 */
class UProgressBar;
class UTextBlock;
class UCanvasPanel;
class UButton;
class UBorder;

UCLASS()
class RPGPROJECT_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:


	virtual bool Initialize()override;

protected:
	//����

	UTextBlock* LevelText;
	//HP MP

	UProgressBar* HPProgressBar;

	UProgressBar* MPProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* HotkeyRowContainer;

	
	

public:
	//5����
	UPROPERTY(meta = (BindWidget))
	UButton* QusetButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SkillButton;
	UPROPERTY(meta = (BindWidget))
	UButton* InventoryButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SettingButton;

	UPROPERTY(meta = (BindWidget))
	class USettingWidget* SettingWidget;

	UPROPERTY(meta = (BindWidget))
	class UMiniMapWidget* Minimap;



	//�����Ϸ�Ѫ��UI
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* EnemyCanvasPanel;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* EnemyHPBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemyHPText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemyNaneLevel;


	//���ܽ���
	UPROPERTY(meta = (BindWidget))
	class UMainTreeWidget* SkillTree;

	UPROPERTY(BlueprintReadOnly)
	TArray<FKey> Keys;  
	UPROPERTY(BlueprintReadOnly)
	TArray<USkillHotkey*>AllHotkeySlots;

	//Buff
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* BuffBox;

	//�������ǲ���ʾ
	bool bTreeShown;



	//������
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ExpBar;
	//��ǰ����
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrnetExpText;
	//��Ҫ�����ľ���
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NeededExpToNextLeveText;
	
	

	
	//����
	UPROPERTY(meta = (BindWidget))
	class USizeBox* QuestListSizeBox;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* QuestList;
	UPROPERTY(meta = (BindWidget))
	class UQuestJournalWidget* QuestJournal;
	

	UPROPERTY(BlueprintReadWrite)
	bool bOutSide = true;
	//��������ϵͳUI
	UPROPERTY(BlueprintReadOnly)
	TArray<class UQuestWidget*>QuestWidgets;
	
	
	//����ϵͳ
	class AQuestManager* QuestManager;

	bool bQuestVisibling=false;




	//����ϵͳ
	UPROPERTY(meta = (BindWidget))
	class UInventoryWidget* InventoryWidget;

	UPROPERTY(meta = (BindWidget))
	class UThrowWidget* ThrowWidget;

	UPROPERTY(meta = (BindWidget))
	class UCraftingMenuWidget*CraftingMenu;

	//�̵�
	class UShopWidget* ShopWidget;

	//�ֿ�
	UPROPERTY(meta = (BindWidget))
	class UStorageWidget* StorageWidget;

	bool bInventoryVisibling = false;

	bool bCraftingVisibling=false;

	UPROPERTY(meta = (BindWidget))
	UBorder*ObtainContainer;

	UPROPERTY(meta = (BindWidget))
	UBorder*ReadableContainer;




	UPROPERTY(meta = (BindWidget))
	class UWrapBox* ItemHotkeyWrapBox;

	class AInventory*Inventory;

	TArray<class UItemHotkeyWidget*> ItemHotkeyWidgets;

	void GenerateItemHotkey(TArray<FKey> ItemKeysToGenerate);

	//���þ���
	void SetLevelText(FText Text);

	void SetHPProgressBar(float Percent);

	void SetMPProgressBar(float Percent);


	//���ɼ��ܰ���  �ⲿ����
	void GenerateHotkeys(TArray<FKey> KeysToGenerate, uint8 KeysPerRow);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<USkillHotkey*>GetAllHotkeySlots() { return AllHotkeySlots; }

	//����ֵ
	void SetExpPanl(float CurrnetExp, float NeededExpToNextLeve);


	//����UI�ؼ�֡���� ħ��ֵ          ��ͼû��д�͵���C++
	UFUNCTION(BlueprintNativeEvent)
	void PlayPopupAnimation();
	void PlayPopupAnimation_Implementation();

	

	


	//���������UI  ��������
	class UQuestWidget* AddQuestToList(class ABaseQuest* Quest);

	//5����
	UFUNCTION()
	void OnQusetButtonClicked();

	UFUNCTION()
	void OnSkillButtonClicked();

	UFUNCTION()
	void OnInventoryButtonClicked();

	         //����
	UFUNCTION()
	void OnSettingButtonClicked();
	bool bSettingVisibling=false;




	TQueue<FInventorySlot>ObtainItemQueue;

	//ʰȡ��Ʒ�ģ�����  *** X ***���й�
	void AddItemToObtainQueue( TSubclassOf <class ABaseItem>ItemClass ,int Amount);

public:
	UFUNCTION(BlueprintCallable)
	void OnObtainMessageEend();

	

protected:
	//��ק����
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


};
