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
	//经验

	UTextBlock* LevelText;
	//HP MP

	UProgressBar* HPProgressBar;

	UProgressBar* MPProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* HotkeyRowContainer;

	
	

public:
	//5个按
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



	//敌人上方血条UI
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* EnemyCanvasPanel;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* EnemyHPBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemyHPText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemyNaneLevel;


	//技能界面
	UPROPERTY(meta = (BindWidget))
	class UMainTreeWidget* SkillTree;

	UPROPERTY(BlueprintReadOnly)
	TArray<FKey> Keys;  
	UPROPERTY(BlueprintReadOnly)
	TArray<USkillHotkey*>AllHotkeySlots;

	//Buff
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* BuffBox;

	//技能树是不显示
	bool bTreeShown;



	//经验条
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ExpBar;
	//当前经验
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrnetExpText;
	//需要升级的经验
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NeededExpToNextLeveText;
	
	

	
	//任务
	UPROPERTY(meta = (BindWidget))
	class USizeBox* QuestListSizeBox;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* QuestList;
	UPROPERTY(meta = (BindWidget))
	class UQuestJournalWidget* QuestJournal;
	

	UPROPERTY(BlueprintReadWrite)
	bool bOutSide = true;
	//所有任务系统UI
	UPROPERTY(BlueprintReadOnly)
	TArray<class UQuestWidget*>QuestWidgets;
	
	
	//任务系统
	class AQuestManager* QuestManager;

	bool bQuestVisibling=false;




	//背包系统
	UPROPERTY(meta = (BindWidget))
	class UInventoryWidget* InventoryWidget;

	UPROPERTY(meta = (BindWidget))
	class UThrowWidget* ThrowWidget;

	UPROPERTY(meta = (BindWidget))
	class UCraftingMenuWidget*CraftingMenu;

	//商店
	class UShopWidget* ShopWidget;

	//仓库
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

	//设置经验
	void SetLevelText(FText Text);

	void SetHPProgressBar(float Percent);

	void SetMPProgressBar(float Percent);


	//生成技能按键  外部调用
	void GenerateHotkeys(TArray<FKey> KeysToGenerate, uint8 KeysPerRow);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<USkillHotkey*>GetAllHotkeySlots() { return AllHotkeySlots; }

	//经验值
	void SetExpPanl(float CurrnetExp, float NeededExpToNextLeve);


	//播放UI关键帧动画 魔法值          蓝图没重写就调用C++
	UFUNCTION(BlueprintNativeEvent)
	void PlayPopupAnimation();
	void PlayPopupAnimation_Implementation();

	

	


	//添加任务到主UI  传入任务
	class UQuestWidget* AddQuestToList(class ABaseQuest* Quest);

	//5个按
	UFUNCTION()
	void OnQusetButtonClicked();

	UFUNCTION()
	void OnSkillButtonClicked();

	UFUNCTION()
	void OnInventoryButtonClicked();

	         //设置
	UFUNCTION()
	void OnSettingButtonClicked();
	bool bSettingVisibling=false;




	TQueue<FInventorySlot>ObtainItemQueue;

	//拾取物品的（动画  *** X ***）有关
	void AddItemToObtainQueue( TSubclassOf <class ABaseItem>ItemClass ,int Amount);

public:
	UFUNCTION(BlueprintCallable)
	void OnObtainMessageEend();

	

protected:
	//拖拽丢弃
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


};
