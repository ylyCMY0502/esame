// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillEnum.generated.h"

/**
 * 
 */


//技能作用选择
UENUM()
enum class ETargetType :uint8
{
	Self,
	SelecteEnemy,//选择伤害
	SelecteArea,//AOE
	AreaAroundSelf,//自身周围
	Missile//远程
};


//技能类型
UENUM(BlueprintType)
enum class EDamageTypeSelected :uint8
{
	Magic,//魔法
	Physical//物理
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
