// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/MissileSkill.h"
#include "Character/RBaseCharacter.h"
#include "Enemy/NormalEnemy.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Character/Skill/MagicProjectile.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"

bool AMissileSkill::BInAttackRange()
{
	return PlayerReference->GetDistanceTo(PlayerReference->SelectedEnemy) <= GetCurrentStage().Rage;
}

void AMissileSkill::InitializeSpellCast()
{
	//�ǲ����ڹ�����Χ�ڣ����ǻ�����CaculateDistanceEnmey ��ѭ���ƶ�
	if (BInAttackRange())
	{
		Super::InitializeSpellCast();
	}
	else
	{
		GetWorldTimerManager().SetTimer(TimerHandle_Caculate,this,&AMissileSkill::CaculateDistanceEnmey,0.1f,true);
		//���ڿ��Ƶ�����ϵ�MissileSkill
		PlayerReference->MissileSkill = this;
	}
}

void AMissileSkill::OnSpellCast()
{
	//ת�����
	PlayerReference->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerReference->GetActorLocation(), PlayerReference->SelectedEnemy->GetActorLocation()));
	Super::OnSpellCast();
}

void AMissileSkill::OnSkillNotify()
{
	//��������ϵͳ������
	//�Ǹ����ܵ�ӵ����
	FActorSpawnParameters Params;
	Params.Owner=this;
	GetWorld()->SpawnActor<AMagicProjectile>(MagicProjectile, PlayerReference->GetMesh()->GetSocketLocation("Muzzle_01"), PlayerReference->GetMesh()->GetSocketRotation("Muzzle_01"), Params);
	//Super::OnSkillNotify();
}

void AMissileSkill::OnTryCastSpell() //�ж��ǲ�������ȴ��
{
	if (PlayerReference->SelectedEnemy)
	{
		Super::OnTryCastSpell();
	}
	
}

void AMissileSkill::CaculateDistanceEnmey()
{
	if (PlayerReference->SelectedEnemy)
	{
		//����ڹ�����Χ �����ʼ����
		if (BInAttackRange())
		{
			InitializeSpellCast();
			//�ڷ�Χ�ھ�ֹͣ
			SotpCaculate();
		}
		else
		{
			//�ƶ���
			UAIBlueprintHelperLibrary::SimpleMoveToActor(PlayerReference->RPlayerController, PlayerReference->SelectedEnemy);
		}
	}
	else
	{
		//���˲����ڣ���ɱ����
		GetWorldTimerManager().ClearTimer(TimerHandle_Caculate);
	}
}

void AMissileSkill::SotpCaculate()
{
	//��ֹΪ�գ�û��GetWorldTimerManager��ֵ�Ǳ�����
	if (TimerHandle_Caculate.IsValid())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Caculate);
	}
	PlayerReference->MissileSkill = nullptr;
}

