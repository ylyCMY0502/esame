// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(disable: 4458)

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillEnum.h"
#include "../RBaseCharacter.h"
#include "Engine/EngineTypes.h"
#include "BaseSkill.generated.h"


class USkillHotkey;


//基本技能
UCLASS()
class RPGPROJECT_API ABaseSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSkill();

protected:

	//USkillEnum
	//技能性息
	UPROPERTY(EditAnywhere,Category="SkillInfo")
	FSkillInfo SkillInfo;

	

	//当前技能序号
	int CurrentStageIndex=-1;

	//是否在冷却中
	bool bOnCooldown;
	//技能是否在释放
	bool bCurrentCasting;

	//与该技能绑定的快捷键
	USkillHotkey* Hotkey;

	//释放技能的角色
	ARBaseCharacter* PlayerReference;

	//技能冷却时间
	float CurrentCD;


	//用于时间计算的
	FTimerHandle  TimerHandle_Cooldown;


	//技能动画蒙太奇
	UPROPERTY(EditAnywhere, Category = "SkillAnim")
	UAnimMontage* SkillAnimMontage;
	//动画实例
	UAnimInstance* AnimInstance;
	FTimerHandle TimerHandle_ResetMove;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//技能初始化，判断技能条件比如说技能魔法值
	virtual void InitializeSpellCast();
	//技能开始释放
	virtual void OnSpellCast();
	//技能释放结束
	virtual void OnCastComleted();
	//技能冷却
	virtual void OnCooldown();
	//技能冷却结束后
	void OnCooldownExpired();

	//播放动画蒙太奇
	virtual void PlaySkillAnimMontage(UAnimMontage* SkillAnim);
	//恢复移动
	void ResetMovement();

	//蓝图调用技能 释放技能
	UFUNCTION(BlueprintCallable)
	virtual void OnSkillNotify();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//返回技能序号 Stages[]
	FORCEINLINE FSkillStage GetCurrentStage() { return SkillInfo.Stages[FMath::Clamp(CurrentStageIndex,0, SkillInfo.Stages.Num()-1)]; }
	//下个技能点数
	FORCEINLINE FSkillStage GetNextStage() { return SkillInfo.Stages[FMath::Clamp(CurrentStageIndex+1, 0, SkillInfo.Stages.Num() - 1)]; }
	//获取当前的阶段
	FORCEINLINE int GetCurrentStageIndex() { return CurrentStageIndex; }
	//改变技能点数
	void ChangeCurrentStageIndex(int DeltaIndex) { CurrentStageIndex = FMath::Clamp(CurrentStageIndex+ DeltaIndex, 0, SkillInfo.Stages.Num() - 1); }

	//设置按键绑定
	FORCEINLINE void SetHotkey(USkillHotkey* Hotkey) { this->Hotkey = Hotkey; }
	FORCEINLINE USkillHotkey* GetHotkey() { return Hotkey; }
	//获取技能性息
	FORCEINLINE FSkillInfo GetSkillInfo() { return SkillInfo;}
	//获取技能是不是在冷却中
	FORCEINLINE bool GetBOnCooldown() { return bOnCooldown; }

	FORCEINLINE ARBaseCharacter* GetPlayerReference() { return PlayerReference; }
	
	FORCEINLINE void SetPlayerReference(ARBaseCharacter* PlayerReference)
	{
		this->PlayerReference = PlayerReference;
	}

	//试图释放技能
	UFUNCTION(BlueprintCallable)
	virtual void OnTryCastSpell();

};
