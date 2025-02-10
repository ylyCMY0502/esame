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


//��������
UCLASS()
class RPGPROJECT_API ABaseSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSkill();

protected:

	//USkillEnum
	//������Ϣ
	UPROPERTY(EditAnywhere,Category="SkillInfo")
	FSkillInfo SkillInfo;

	

	//��ǰ�������
	int CurrentStageIndex=-1;

	//�Ƿ�����ȴ��
	bool bOnCooldown;
	//�����Ƿ����ͷ�
	bool bCurrentCasting;

	//��ü��ܰ󶨵Ŀ�ݼ�
	USkillHotkey* Hotkey;

	//�ͷż��ܵĽ�ɫ
	ARBaseCharacter* PlayerReference;

	//������ȴʱ��
	float CurrentCD;


	//����ʱ������
	FTimerHandle  TimerHandle_Cooldown;


	//���ܶ�����̫��
	UPROPERTY(EditAnywhere, Category = "SkillAnim")
	UAnimMontage* SkillAnimMontage;
	//����ʵ��
	UAnimInstance* AnimInstance;
	FTimerHandle TimerHandle_ResetMove;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//���ܳ�ʼ�����жϼ�����������˵����ħ��ֵ
	virtual void InitializeSpellCast();
	//���ܿ�ʼ�ͷ�
	virtual void OnSpellCast();
	//�����ͷŽ���
	virtual void OnCastComleted();
	//������ȴ
	virtual void OnCooldown();
	//������ȴ������
	void OnCooldownExpired();

	//���Ŷ�����̫��
	virtual void PlaySkillAnimMontage(UAnimMontage* SkillAnim);
	//�ָ��ƶ�
	void ResetMovement();

	//��ͼ���ü��� �ͷż���
	UFUNCTION(BlueprintCallable)
	virtual void OnSkillNotify();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//���ؼ������ Stages[]
	FORCEINLINE FSkillStage GetCurrentStage() { return SkillInfo.Stages[FMath::Clamp(CurrentStageIndex,0, SkillInfo.Stages.Num()-1)]; }
	//�¸����ܵ���
	FORCEINLINE FSkillStage GetNextStage() { return SkillInfo.Stages[FMath::Clamp(CurrentStageIndex+1, 0, SkillInfo.Stages.Num() - 1)]; }
	//��ȡ��ǰ�Ľ׶�
	FORCEINLINE int GetCurrentStageIndex() { return CurrentStageIndex; }
	//�ı似�ܵ���
	void ChangeCurrentStageIndex(int DeltaIndex) { CurrentStageIndex = FMath::Clamp(CurrentStageIndex+ DeltaIndex, 0, SkillInfo.Stages.Num() - 1); }

	//���ð�����
	FORCEINLINE void SetHotkey(USkillHotkey* Hotkey) { this->Hotkey = Hotkey; }
	FORCEINLINE USkillHotkey* GetHotkey() { return Hotkey; }
	//��ȡ������Ϣ
	FORCEINLINE FSkillInfo GetSkillInfo() { return SkillInfo;}
	//��ȡ�����ǲ�������ȴ��
	FORCEINLINE bool GetBOnCooldown() { return bOnCooldown; }

	FORCEINLINE ARBaseCharacter* GetPlayerReference() { return PlayerReference; }
	
	FORCEINLINE void SetPlayerReference(ARBaseCharacter* PlayerReference)
	{
		this->PlayerReference = PlayerReference;
	}

	//��ͼ�ͷż���
	UFUNCTION(BlueprintCallable)
	virtual void OnTryCastSpell();

};
