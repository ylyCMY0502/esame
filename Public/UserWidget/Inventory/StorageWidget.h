// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StorageWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class RPGPROJECT_API UStorageWidget : public UUserWidget
{
	GENERATED_BODY()
	

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseButtonClicke();


protected:

	//UPROPERTY(EditAnywhere, Category = "UI")
	//TSubclassOf<class UDragDropOperation>InventoryDragDropOperation;

	//virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

public:

	UPROPERTY(meta = (BindWidget))
		UButton* CloseButton;



	

	UPROPERTY(meta = (BindWidget))
	class UWrapBox* StorageBox;




	class AStorage* StorageRef;
	class AInventory* PlayerInventoryRef;

	TArray<class UStorageSlotWidget*> StorageSlotWidgets;
	void GenerateSlotWidget(class AStorage* StorageRef);


	/*UFUNCTION()
	void OnSortButtonClicked();*/

	bool bReversed = false;


	void UpdateThisSlots();
	
	void OnSlotClicked(int Index);


};
