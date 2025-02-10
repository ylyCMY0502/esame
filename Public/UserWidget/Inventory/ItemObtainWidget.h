// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemObtainWidget.generated.h"

/**
 * 
 */

 class UTextBlock;

UCLASS()
class RPGPROJECT_API UItemObtainWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UImage*ImageIocn;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock*AmountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* XText;
  
	TSubclassOf<class ABaseItem>BaseItemClass;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VisibleDuration=1.2f;

	UPROPERTY(BlueprintReadOnly)
	class UMainUserWidget* MainUserWidget;
public:


	void Init(TSubclassOf<class ABaseItem> ItemClass,int Amount, class UMainUserWidget* MainUserWidget);
};
