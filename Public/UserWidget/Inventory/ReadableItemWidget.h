// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadableItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UReadableItemWidget : public UUserWidget
{
	GENERATED_BODY()


	UPROPERTY(meta=(BindWidget))
	class UTextBlock*NameText;

	UPROPERTY(meta = (BindWidget))
	class UImage*MapImage;

	UPROPERTY(meta = (BindWidget))
	class UButton*CloseButton;

	UFUNCTION()
	void OnCloseButtonClicked();

	virtual void NativeConstruct();

public:
	class AItemMap* ItemMap;
	
};
