// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuffWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API UBuffWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	UPROPERTY(meta = (BindWidget))
	UImage* BuffIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* CooldownImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CooldownTime;


	//UPROPERTY(EditAnywhere)
	class UTexture2D* BuffTexture;
	
	//用于设置时间变化
	class UMaterialInstanceDynamic* CooldownMat;

	virtual void NativeConstruct()override;
};
