// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BaseSkill.h"
#include "SkillNova.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API ASkillNova : public ABaseSkill
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,Category="ParticleSystem")
	class UParticleSystem* ParticleSystem;

	//重写蒙太奇技能通知
	virtual void OnSkillNotify()override;
	
};
