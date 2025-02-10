// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Character/Skill/SkillEnum.h"
#include "DamageableInterface.generated.h"

class ABaseSkill;
class ABaseElement;



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGPROJECT_API IDamageableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//（基础伤害的值，伤害的类型，技能元素，暴击率，那个对象，那个技能）
	virtual void OnReceiveDamage(float BaseDamage,EDamageTypeSelected Type,TSubclassOf<ABaseElement>Element,int CritChance,AActor*Attacker, ABaseSkill* Skill)=0;
};
