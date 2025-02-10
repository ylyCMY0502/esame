// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConnectionWidget.generated.h"

/**
 * 技能树下线
 */
UCLASS()
class RPGPROJECT_API UConnectionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* LineDown;

	//前置技能的数组
	TSubclassOf<class ABaseSkill>ForSpell;
	
	//连线是不是被激活
	bool bActivte;
	//连线激活
	void Activte();
	
};
