// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecipeEntryWidget.generated.h"

/**
 * 
 */

class UTextBlock;

UCLASS()
class RPGPROJECT_API URecipeEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock*NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NumText;

	UPROPERTY(meta = (BindWidget))
	class UImage*IconImage;

public:

	class UCraftingMenuWidget* CraftingMenuWidget;

	TSubclassOf<class ABaseItem>Item;
	int RequiredAmount;
	int	CurrentAmount;

	
	void Update();
};
