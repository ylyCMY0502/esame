// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/SkillNova.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Enemy/NormalEnemy.h"
#include "Character/RBaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"



void ASkillNova::OnSkillNotify()
{
	//播放粒子动画
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ParticleSystem, PlayerReference->GetMesh()->GetSocketLocation("foot_l"), PlayerReference->GetActorRotation());
	
	
	//技能伤害啊
	FCollisionShape CollisionShape =FCollisionShape::MakeSphere(GetCurrentStage().Rage);
	//UKismetSystemLibrary::DrawDebugSphere(this, PlayerReference->GetActorLocation(), GetCurrentStage().Rage, 12, FLinearColor::Blue,5.0f,2.0f);
	TArray <FHitResult> HitResults;
	if (GetWorld()->SweepMultiByChannel(HitResults, PlayerReference->GetActorLocation(), PlayerReference->GetActorLocation(), FQuat::Identity, ECC_Visibility, CollisionShape))
	{
		//UE_LOG(LogTemp, Warning, TEXT("..........GetWorld"));
		for (auto HitResult: HitResults)
		{
			if (HitResult.GetActor())
			{
				ANormalEnemy* Enemy = Cast<ANormalEnemy>(HitResult.GetActor());
				if (Enemy)
				{
					IDamageableInterface * DamageableInterface =Cast<IDamageableInterface>(Enemy);
					if (DamageableInterface)
					{
						DamageableInterface->OnReceiveDamage(GetCurrentStage().Damage, GetCurrentStage().DamageType, GetSkillInfo().Element, GetCurrentStage().CriticalChance,PlayerReference,this);
					}
				}
			}
		}
	}
}
