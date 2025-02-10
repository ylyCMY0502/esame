// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UEnemyInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* LevelName;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar; 

	
};
