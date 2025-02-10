// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Skill/SkillEnum.h"
#include "Interface/DamageableInterface.h"
#include "Interface/SelectableInterface.h"
#include "NormalEnemy.generated.h"

class ABaseElement;


UCLASS()
class RPGPROJECT_API ANormalEnemy : public ACharacter ,public IDamageableInterface ,public ISelectableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANormalEnemy();

	class ANormalEnmeyController* MyController;

	//是否有侵略性的
	UPROPERTY(EditAnywhere,Category="Info")
	bool bAggressive;

	//ai
	UPROPERTY(EditAnywhere,Category="AI")
	class UAIPerceptionComponent* AIPerceptionCom;
	class UAISenseConfig_Sight* SightConfig;

	//元素
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ABaseElement>Element;

	//加载属性
	/*UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget>EnemyWidgetInfo;
	*/
	UPROPERTY(EditAnywhere, Category = "UI")
	class UWidgetComponent* EnemyWidgetCom;

	UPROPERTY(EditAnywhere, Category = "UI")
	class USphereComponent* ShowUICollision;

	UPROPERTY(EditAnywhere, Category = "Info")
	float TotalHealth = 100;

	//UPROPERTY(EditAnywhere, Category = "UI")
	float CurrentHealth ;

protected:
	//基础属性
	UPROPERTY(EditAnywhere, Category = "Skill")
	float BaseDamage = 20.0f;
	UPROPERTY(EditAnywhere, Category = "Skill")
	TEnumAsByte<EDamageTypeSelected> DamageType;
	UPROPERTY(EditAnywhere, Category = "Skill")
	int CritChance=10;

	class UEnemyInfoWidget* EnemyInfoWidget;

	UPROPERTY(EditAnywhere, Category = "Info")
	FText Name = FText::FromString("Spider");
	UPROPERTY(EditAnywhere, Category = "Info")
	int Level =2;
	
	//经验值
	UPROPERTY(EditAnywhere, Category = "Info")
	float ExpForKill;
	UPROPERTY(EditAnywhere, Category = "Info")
	bool bDoseRespawn=true;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//是否死亡
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Behavior")
	bool bDead;

	//关于视觉感知更新
	UFUNCTION()
	void OnSightPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	//蓝图调用受伤函数
	UFUNCTION(BlueprintCallable)
	void NotifyHit();
	//攻击
	void AttackRay();    

	//用于UI碰撞显示
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//初始化 UI 文本 名字登记
	void InitWidgetText();
	
	//死亡
	void OnDeath(AActor* Killer);
	//死亡后延迟 用于播放死亡动画流出时间
	void OnDelayDeath();
	//死亡延迟计时
	FTimerHandle Timehandle_DelayDeath;
	UPROPERTY(EditAnywhere, Category = "Anim")
	float DelayDeath = 2.0f;


	//重生
	void Respawn();
	
	FTimerHandle Timehandle_Respawn;
	float RespawnTime = 10;


	//是否选择
	bool bSelected;
	class ARBaseCharacter* SelectingCharacter;
	//选择上
	virtual void OnSelectable(class ARBaseCharacter* Character) override;
	//鼠标离开
	virtual void OnSelectionEnd(class ARBaseCharacter* Character) override;


public:	

	//出生的位置
	FVector StartLocation;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	TArray<class UAnimMontage*>AttackAnimations;

	bool bInShowRange;
	
	//弹道目标
	UPROPERTY(EditAnywhere, Category = "Hit")
	class UArrowComponent* HitArrowCom;

	//获取死亡
	FORCEINLINE bool GetBDead() {return bDead;}
	//更新HP
	void UpdateHealthBar();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnReceiveDamage(float BaseDamage, EDamageTypeSelected Type, TSubclassOf<ABaseElement>Element, int CritChance, AActor* Attacker, ABaseSkill* Skill) override;


};
