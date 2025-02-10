// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BaseSkill.h"
#include "BuffSkill.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API ABuffSkill : public ABaseSkill
{
	GENERATED_BODY()

public:
	class UBuffWidget* BuffWidgetRef;

	UPROPERTY(EditAnywhere)
	class UTexture2D*BuffIcon;
	
	//Buff�ӳ�ʱ��
	float BuffDuration;
	//ʣ��ʱ��
	float RemainingTime;

	FTimerHandle TimerHandl_UpdateEffect;


	//buffЧ���ͼ�
	virtual void ApplyEffect();
	//buff�Ƴ�
	virtual void RemoveEffect();
	//buff����
	virtual void RestBuff();
	//buff���������ܵ���ڣ����� ApplayEffect ֮�⣬����������Buff֮�������
	virtual void OnActive();
	//buffʧЧ
	virtual void OnDeActive();
	//��ʱ��
	virtual void OnUpdateBuff();

	virtual void OnSkillNotify() override;

};
