// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSkill.h"
#include "MissileSkill.generated.h"

/**
 * 
 */
class UParticleSystem;
class UNiagaraSystem;

UCLASS()
class RPGPROJECT_API AMissileSkill : public ABaseSkill
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class AMagicProjectile>MagicProjectile;



protected:

	FTimerHandle TimerHandle_Caculate;

	//判断是不是在攻击范围内
	bool BInAttackRange();
	//技能初始化，判断技能条件比如说技能魔法值
	virtual void InitializeSpellCast() override;
	//技能开始释放
	virtual void OnSpellCast() override;
	//蓝图调用技能 释放技能
	virtual void OnSkillNotify() override;
	//试图释放技能
	virtual void OnTryCastSpell() override;
	//移动到攻击范围馁
	void CaculateDistanceEnmey();



public:
	//粒子系统
	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* MissileEffect;
	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ImpactEffect;

	//Niagara
	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* MissileNiagaraEffect;
	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* ImpactNiagaraEffect;


	void SotpCaculate();
};
