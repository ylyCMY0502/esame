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
	
	//Buff加持时间
	float BuffDuration;
	//剩下时间
	float RemainingTime;

	FTimerHandle TimerHandl_UpdateEffect;


	//buff效果释加
	virtual void ApplyEffect();
	//buff移除
	virtual void RemoveEffect();
	//buff重置
	virtual void RestBuff();
	//buff激活，这个技能的入口，出了 ApplayEffect 之外，还有完成添加Buff之类的事情
	virtual void OnActive();
	//buff失效
	virtual void OnDeActive();
	//定时器
	virtual void OnUpdateBuff();

	virtual void OnSkillNotify() override;

};
