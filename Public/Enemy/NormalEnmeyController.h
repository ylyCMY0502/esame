// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NormalEnmeyController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API ANormalEnmeyController : public AAIController
{
	GENERATED_BODY()



	class ANormalEnemy* EnemyPawn;

	class UNavigationSystemV1* NavSys;

	FTimerHandle TimerHandle_Patrol;


	//追击的速度
	float AggroedWalkSpeed = 600.0f;
	
	//最大跟随目标的距离
	UPROPERTY(EditAnywhere,Category="AI")
	float MaxDistanceToFollowTarget = 1500.0f;


	
	//当前攻击的动画
	class UAnimMontage* CurrentAttackMontage;
	
	
	//小于这个范围攻击
	float AttackRange=300.0f;



	ANormalEnmeyController();

	//播放下一个动画的索引
	int GetNextAnimationIndex();
	//判断在不在攻击范围
	bool BInAttackRange();

	//传入ai 控制的角色
	virtual void OnPossess(APawn* InPawn) override;

	//移动完成会自动调用
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	//是否在巡逻中调用 Patrol()
	void DetectedPatrol();
	
	//攻击动画
	void PerforAttack();

	FTimerHandle TimerHandle_OnAnimPlayOver;
	//攻击动画播放完毕调用
	void OnAnimPlayOver();
	
	//用于判断追击的距离是不是在指定的追击范围内
	void CacualteTargetDistance();

	//追击的距离大于最大值时，重置为初始的状态
	void OnReset();
	
	FTimerHandle TimerHandle_CalDis;
public:

	//动画实例
	UAnimInstance* AnimInstance;

	//是否在巡逻
	bool bIsPatrolling = true;

	//巡逻速度
	float PatroyWalkSpeed = 200.0f;

	//攻击追击的对象
	AActor* TargetActor;
	//追击的目标是否脱离
	bool bIsRunningBack;


	//当前攻击动画序号
	int CurrentAttackIndex = 0;

	//巡逻
	void Patrol();


	//是否在狂暴状态
	bool bWasAggroed = false;
	//发现目标后追逐
	void OnAggroedPulled(AActor* Target);

	
};
