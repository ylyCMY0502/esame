// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BuffSkill.h"
#include "BuffSkill_Health.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API ABuffSkill_Health : public ABuffSkill
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float AddHP;

	virtual void ApplyEffect()override;
	
};
