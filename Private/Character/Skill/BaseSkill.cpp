// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/BaseSkill.h"

#include "UserWidget/SkillHotkey.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Button.h"
#include "UserWidget/MainUserWidget.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "Animation/WidgetAnimation.h"

// Sets default values
ABaseSkill::ABaseSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseSkill::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerReference = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	AnimInstance = PlayerReference->GetMesh()->GetAnimInstance();
}





void ABaseSkill::InitializeSpellCast()
{
	if (PlayerReference->GetCurrentMP() >= GetCurrentStage().ManaCost)
	{
		bCurrentCasting = true;
		PlayerReference->BeginSpellCast(this);

		PlayerReference->ChangeCurrentMP(GetCurrentStage().ManaCost * -1);
		OnSpellCast();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("������"));
		PlayerReference->MainUserWidget->PlayPopupAnimation();
		
	}
}

void ABaseSkill::OnSpellCast()
{
	

	UE_LOG(LogTemp, Warning, TEXT("Casting skill: %s"), *SkillInfo.Name.ToString());
	//������������ڵĻ� ���ü��ܲ���
	if (SkillAnimMontage)
	{
		PlaySkillAnimMontage(SkillAnimMontage);
	}

	OnCastComleted();
}


void ABaseSkill::OnCastComleted()
{
	//��ɫ�еļ����ͷŽ���
	PlayerReference->EndSpellCast(this);
	//����û�����ͷ�
	bCurrentCasting = false;
	//��������ȴ��
	bOnCooldown =true ;
	
	//UE_LOG(LogTemp, Warning, TEXT("Hotkey-%d"), Hotkey);

	if (Hotkey)
	{
		Hotkey->SkillButton->SetIsEnabled(false);
		Hotkey->SkillIcon->SetColorAndOpacity(FLinearColor(0.33, 0.33, 0.33, 1));
		Hotkey->CooldownMat->SetScalarParameterValue("Percent", 1);
	
		Hotkey->CooldownImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		Hotkey->CooldownText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		CurrentCD = GetCurrentStage().CooldownTime;
		//ʱ��                                                       �󶨵ĺ���         ����ʱ�����һ��  �ǲ���Ҫѭ������             
		GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ABaseSkill::OnCooldown,0.01f,true);

	}

}

void ABaseSkill::OnCooldown()
{
	
	if (CurrentCD <= 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Cooldown);

		OnCooldownExpired();
	}
	else
	{
		CurrentCD -= 0.01f;

		
		Hotkey->CooldownMat->SetScalarParameterValue("Percent", CurrentCD/GetCurrentStage().CooldownTime);
		
		if (CurrentCD >= 1.0f)
		{
			Hotkey->CooldownText->SetText(FText::AsNumber(FMath::RoundFromZero(CurrentCD)));
		}
		else
		{
			Hotkey->CooldownText->SetText(FText::AsNumber(FMath::RoundFromZero(10.0f*CurrentCD)/10));
		}
	}
}

void ABaseSkill::OnCooldownExpired()
{
	
	bOnCooldown = false;
	
	if (Hotkey)
	{
		if (!Hotkey->bDeactived)
		{
			Hotkey->SkillButton->SetIsEnabled(true);
			Hotkey->SkillIcon->SetColorAndOpacity(FLinearColor::White);

		}

		Hotkey->CooldownMat->SetScalarParameterValue("Percent", 0);
		Hotkey->CooldownImage->SetVisibility(ESlateVisibility::Hidden);
		Hotkey->CooldownText->SetVisibility(ESlateVisibility::Hidden);
	}
}




void ABaseSkill::PlaySkillAnimMontage(UAnimMontage* SkillAnimation)
{
	//UCharacterMovementComponent �ƶ�ʧЧ
	PlayerReference->GetCharacterMovement()->DisableMovement();
	//����ֹͣ�˶�
	PlayerReference->GetCharacterMovement()->StopMovementImmediately();
	AnimInstance->Montage_Play(SkillAnimation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetMove, this, &ABaseSkill::ResetMovement, SkillAnimation->GetPlayLength(), false);

}


//�ָ��ƶ�״̬
void ABaseSkill::ResetMovement()
{
	PlayerReference->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}


//û����д�������
void ABaseSkill::OnSkillNotify()
{
	UE_LOG(LogTemp, Warning, TEXT("OnSkillNotify null"));
}

// Called every frame
void ABaseSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseSkill::OnTryCastSpell()
{
	/*UE_LOG(LogTemp, Warning, TEXT("OnTryCastSpell 0,0,0"));
	UE_LOG(LogTemp, Warning, TEXT("OnTryCastSpell bOnCooldown-%d"),bOnCooldown);
	UE_LOG(LogTemp, Warning, TEXT("OnTryCastSpell bCurrentCasting-%d"), bCurrentCasting);
	UE_LOG(LogTemp, Warning, TEXT("OnTryCastSpell PlayerReference->GetBIsCasting-%d"), PlayerReference->GetBIsCasting());
	*/
	//������ܲ�����ȴ�� ����û���ͷ�          ��ɫû���ͷ�
	if (!bOnCooldown && !bCurrentCasting && !PlayerReference->GetBIsCasting())
	{
		InitializeSpellCast();
	}
}

