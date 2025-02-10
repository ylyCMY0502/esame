// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftableItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UCraftableItemWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock*NameText;


	class UCraftingMenuWidget* CraftingMenuWidget;

	

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* SelectedButton;

	TSubclassOf<class ABaseItem>Item;

	virtual void NativeConstruct()override;
	void Init(class UCraftingMenuWidget*CraftingMenuWidget, TSubclassOf<class ABaseItem>Item);
	
	UFUNCTION()
	void OnSelectedButtonClicked();
};
