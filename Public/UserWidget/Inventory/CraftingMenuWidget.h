// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryStruct.h"
#include "CraftingMenuWidget.generated.h"

/**
 * 
 */

class UButton;
class UTextBlock;

UCLASS()
class RPGPROJECT_API UCraftingMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

	UPROPERTY(meta = (BindWidget))
	UButton*CloseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CraftButton;

	UPROPERTY(meta = (BindWidget))
	UButton*CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox*CraftaleItemScrollBox;

	UPROPERTY(meta = (BindWidget))
	class UImage*IconImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionText;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox*RecipeEntriesList;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox*DetailBox;

public:
	class AInventory* Inventory;

	virtual void NativeConstruct()override;

	UFUNCTION()
	void OnCloseButtonClicked();

	UFUNCTION()
	void OnCraftButtonClicked();
	//
	void OnCraftableCliked(class UCraftableItemWidget*ClickedItem);
	//����
	void GenerateCraftaleList();
	class UCraftableItemWidget* CurrentCraftableWidget;
	TSubclassOf<class ABaseItem>CurrentItem;
	FItemInfo CurrentItemInfo;
	TArray<class URecipeEntryWidget*> RecipeEntryWidgets;
	void GenerateRecipeEntrys();
	//�ǲ����Ժϳ�
	bool BCanBeCrafted();
	//�����Ǹ��ϳ���Ʒ����Ϣ
	void UpdateDetailWindow(TSubclassOf<class ABaseItem> Item);

	//��ʼ���˵�
	void IntiCraftaingMenu(class AInventory*Inventory);
};
