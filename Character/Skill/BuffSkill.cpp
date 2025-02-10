// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/BuffSkill.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "TimerManager.h"
#include "UserWidget/BuffWidget.h"
#include "Materials/MaterialInstanceDynamic.h"

void ABuffSkill::ApplyEffect()
{

}

void ABuffSkill::RemoveEffect()
{

}

void ABuffSkill::RestBuff()
{
	BuffDuration = GetCurrentStage().CastingTime;
	GetWorldTimerManager().ClearTimer(TimerHandl_UpdateEffect);
	OnActive();
}

void ABuffSkill::OnActive()
{
	BuffWidgetRef = PlayerReference->AddBuff(this);
	BuffDuration = GetCurrentStage().CastingTime;

	BuffWidgetRef->CooldownImage->SetVisibility(ESlateVisibility::Visible);
	BuffWidgetRef->CooldownTime->SetVisibility(ESlateVisibility::Visible);

	RemainingTime = BuffDuration;
	GetWorldTimerManager().SetTimer(TimerHandl_UpdateEffect,this,&ABuffSkill::OnUpdateBuff,0.01,true);

}

void ABuffSkill::OnDeActive()
{
	RemoveEffect();
	PlayerReference->RemoveBuff(this);
	BuffWidgetRef = nullptr;

}

void ABuffSkill::OnUpdateBuff()
{
	if (RemainingTime<=0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandl_UpdateEffect);
		OnDeActive();
	} 
	else
	{
		ApplyEffect();
		RemainingTime -= 0.01f;
		BuffWidgetRef->CooldownTime->SetText(FText::AsNumber(FMath::RoundFromZero(RemainingTime)));
		BuffWidgetRef->CooldownMat->SetScalarParameterValue("Percent", RemainingTime / BuffDuration);
	}
	
}

void ABuffSkill::OnSkillNotify()
{
	Super::OnSkillNotify();
	if (PlayerReference->Buffs.Contains(this))
	{
		RestBuff();
	} 
	else
	{
		OnActive();
	}
	
}
