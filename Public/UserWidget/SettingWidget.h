// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingWidget.generated.h"

class UButton;

UCLASS()
class RPGPROJECT_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()


	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	UButton*SaveButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

protected:
	virtual void NativeConstruct()override;
	
	UFUNCTION()
	void OnSaveButtonClicked();
	UFUNCTION()
	void OnLoadButtonClicked();

	UFUNCTION()
		void OnCloseButtonClicked();
public:
	class ARBaseCharacter*PlayerCharacter;
	
};
