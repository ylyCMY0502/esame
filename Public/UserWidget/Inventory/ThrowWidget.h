// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../InventorySystem/InventoryStruct.h"
#include "ThrowWidget.generated.h"


class UTextBlock;
class UButton;


UCLASS()
class RPGPROJECT_API UThrowWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;

	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NumText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountText;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* CountEditableText;


	UPROPERTY(meta = (BindWidget))
	UButton* MinusButton;
	UPROPERTY(meta = (BindWidget))
	UButton* PlusButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;



public:
	//当前要丢弃的数量
	int ThrowCount;
	//插槽上的数量
	int MaxAmout;
	//当前调用的插槽是那个
	int CurrentIndex;

	class AInventory* InventoryRef;
	FItemInfo ItemInfo;

	int ClickCount;
	FTimerHandle TimerHandle_CountToZero;
	FTimerHandle TimeHanlde_Increase;
	FTimerHandle TimeHanlde_Decrease;

	virtual void NativeConstruct()override;



	void Update(int Index);

	//-
	void IncreaseCount();
	//+
	void DecreaseCount();

	UFUNCTION()
	void OnMinusButtonClicked();
	UFUNCTION()
	void OnPlusButtonClicked();

	UFUNCTION()
	void OnMinusButtonPressed();
	UFUNCTION()
	void OnPlusButtonPressed();


	UFUNCTION()
	void OnMinusButtonReleased();
	UFUNCTION()
	void OnPlusButtonReleased();
	
	void SetClickCountToZero();

	UFUNCTION()
	void OnConfirmButtonClicked();
	UFUNCTION()
	void OnCancelButtonClicked();

};
