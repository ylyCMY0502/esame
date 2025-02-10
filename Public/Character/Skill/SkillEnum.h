// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillEnum.generated.h"

/**
 * 
 */


//��������ѡ��
UENUM()
enum class ETargetType :uint8
{
	Self,
	SelecteEnemy,//ѡ���˺�
	SelecteArea,//AOE
	AreaAroundSelf,//������Χ
	Missile//Զ��
};


//��������
UENUM(BlueprintType)
enum class EDamageTypeSelected :uint8
{
	Magic,//ħ��
	Physical//����
};


//bufll
UENUM()
enum class EEffectiveness :uint8
{
	Effective,
	SuperEffective,
	LowEffective
};



UCLASS()
class RPGPROJECT_API USkillEnum : public UObject
{
	GENERATED_BODY()
	
};
