// Fill out your copyright notice in the Description page of Project Settings.


#include "Other/StaticLibrary.h"
#include "Character/Skill/BaseElement.h"
#include "Character/Skill/SkillEnum.h"
#include "Enemy/NormalEnemy.h"

float UStaticLibrary::CaculateFinalDamage(float BaseDamage, int CritChance, TSubclassOf<ABaseElement>AttackerElement, TSubclassOf<ABaseElement>DefenderElement)
{
	float CurrentDamage = FMath::RandRange(0.9f,1.1f) * BaseDamage;
	EEffectiveness LocalEffectiveness;

	//�����ߣ����ܣ�  ������
	if (AttackerElement && DefenderElement)
	{
		if (DefenderElement->GetDefaultObject<ABaseElement>()->ElementInfo.Weakness.Contains(AttackerElement))
		{
			LocalEffectiveness= EEffectiveness::LowEffective;
		} 
		else if(DefenderElement->GetDefaultObject<ABaseElement>()->ElementInfo.Enhance.Contains(AttackerElement))
		{
			LocalEffectiveness = EEffectiveness::SuperEffective;
		}
		else
		{
			LocalEffectiveness = EEffectiveness::Effective;
		}
	} 
	else
	{
		LocalEffectiveness = EEffectiveness::Effective;
	}

	switch (LocalEffectiveness)
	{
	case EEffectiveness::Effective:
		BaseDamage = BaseDamage; 
		break;
	case EEffectiveness::SuperEffective:
		BaseDamage = BaseDamage * 1.5f;
		break;
	case EEffectiveness::LowEffective:
		BaseDamage = BaseDamage * 0.7f;
		break;
	default:
		break;
	}

	if (FMath::RandHelper(101) <= CritChance)
	{
		BaseDamage *= 2;
	}
	

	return BaseDamage;
}

bool UStaticLibrary::BIsEnemy(AActor* Actor)
{
	return (Actor->GetClass() == ANormalEnemy::StaticClass() || Actor->GetClass()->IsChildOf(ANormalEnemy::StaticClass()));
}
