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

	//�ж��ǲ����ڹ�����Χ��
	bool BInAttackRange();
	//���ܳ�ʼ�����жϼ�����������˵����ħ��ֵ
	virtual void InitializeSpellCast() override;
	//���ܿ�ʼ�ͷ�
	virtual void OnSpellCast() override;
	//��ͼ���ü��� �ͷż���
	virtual void OnSkillNotify() override;
	//��ͼ�ͷż���
	virtual void OnTryCastSpell() override;
	//�ƶ���������Χ��
	void CaculateDistanceEnmey();



public:
	//����ϵͳ
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
