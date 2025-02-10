// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillDrag.generated.h"


/**
 * 
 */

class UTexture2D;


UCLASS()
class RPGPROJECT_API USkillDrag : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UImage* SkillIcon;
	

public:
	void SetSkillTexture(UTexture2D* Texture);
};
