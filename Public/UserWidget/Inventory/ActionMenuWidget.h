// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../InventorySystem/InventoryStruct.h"
#include "ActionMenuWidget.generated.h"

class UButton;

UCLASS()
class RPGPROJECT_API UActionMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	UButton* UseItemButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ThrowAwayButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SplitStacksButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CancleButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UseText;


public:
	class AInventory* InventoryRef;
	int CurrentIdex;
	FItemInfo ItemInfo;
	int Amount;


	virtual void NativeConstruct() override;

	//¸ü½çÃæ
	void Update(int Index);

	UFUNCTION()
	void OnUseItemButtonClicked();

	UFUNCTION()
	void OnThrowAwayButtonClicked();

	UFUNCTION()
	void OnSplitStacksButtonClicked();

	UFUNCTION()
	void OnCancleButtonClicked();


};
