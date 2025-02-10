// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NormalEnmeyController.h"
#include "Enemy/NormalEnemy.h"
#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"


ANormalEnmeyController::ANormalEnmeyController()
{
	//��õ�ǰ��
	NavSys =FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
}

void ANormalEnmeyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyPawn = Cast<ANormalEnemy>(InPawn);

	AnimInstance = EnemyPawn->GetMesh()->GetAnimInstance();
}



int ANormalEnmeyController::GetNextAnimationIndex()
{
	if (CurrentAttackIndex+1 >= EnemyPawn->AttackAnimations.Num())
	{
		return 0;
	}
	else
	{
		return CurrentAttackIndex + 1;
	}
}




void ANormalEnmeyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (bIsRunningBack)
	{
		bIsRunningBack = false;
		bIsPatrolling = false;
		EnemyPawn->GetCharacterMovement()->MaxWalkSpeed = PatroyWalkSpeed;
	}
	
	
	if (BInAttackRange())
	{
		PerforAttack();
		return;
	}


	if (bIsPatrolling)
	{
		float ThinkTime = FMath::RandRange(1, 4);
		
		//����DetectedPatrol 
		GetWorldTimerManager().SetTimer(TimerHandle_Patrol, this, &ANormalEnmeyController::DetectedPatrol, ThinkTime,false);
	}

}


bool ANormalEnmeyController::BInAttackRange()
{
	if (TargetActor)
	{
		return (EnemyPawn->GetDistanceTo(TargetActor) <= AttackRange) ;
	} 
	else
	{
		return false;
	}
}


void ANormalEnmeyController::DetectedPatrol()
{
	if (bIsPatrolling)
	{
		Patrol();
	}
}



void ANormalEnmeyController::PerforAttack()
{
	if (!EnemyPawn->GetBDead() && !bIsRunningBack && bWasAggroed)
	{
		bIsPatrolling = false;
		//UCharacterMovementComponent
		EnemyPawn->GetCharacterMovement()->StopActiveMovement();

		if (TargetActor)
		{
			//������ת�ĽǶ�
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(EnemyPawn->GetActorLocation(), TargetActor->GetActorLocation());
			EnemyPawn->SetActorRotation(Rotation);
		}

		CurrentAttackMontage = EnemyPawn->AttackAnimations[CurrentAttackIndex];
		AnimInstance->Montage_Play(CurrentAttackMontage);

		GetWorldTimerManager().SetTimer(TimerHandle_OnAnimPlayOver, this, &ANormalEnmeyController::OnAnimPlayOver, CurrentAttackMontage->GetPlayLength(), false);
	}

}

void ANormalEnmeyController::OnAnimPlayOver()
{
	CurrentAttackMontage = nullptr;
	CurrentAttackIndex = GetNextAnimationIndex();

	
	//�ж��ǲ����ڹ�����Χ��
	if (BInAttackRange())
	{
		PerforAttack();
	}
	//���ڹ�����Χ�˾��ƶ���ȥ
	else
	{
		MoveToActor(TargetActor);
	}
}

void ANormalEnmeyController::OnAggroedPulled(AActor* Target)
{
	if (!bWasAggroed && !bIsRunningBack)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnAggroedPulled"));
		bWasAggroed = true;
		TargetActor = Target;
		bIsPatrolling = false;

		EnemyPawn->GetCharacterMovement()->MaxWalkSpeed = AggroedWalkSpeed;

		GetWorldTimerManager().SetTimer(TimerHandle_CalDis,this,&ANormalEnmeyController::CacualteTargetDistance,1.0f,true);

		//�ж��ǲ����ڹ�����Χ��
		if (BInAttackRange())
		{
			PerforAttack();
		}
		//���ڹ�����Χ�˾��ƶ���ȥ
		else
		{
			MoveToActor(TargetActor);
		}

	}

}

void ANormalEnmeyController::CacualteTargetDistance()
{

	if(TargetActor)
	{ 
		if (FVector::Dist(EnemyPawn->GetActorLocation(),TargetActor->GetActorLocation())> MaxDistanceToFollowTarget)
		{
			//����Ϊ��ʼ״̬
			OnReset();
		}
	}
}

void ANormalEnmeyController::OnReset()
{
	//0.0f��ֹͣ���Ŷ���
	AnimInstance->Montage_Stop(0.0f);
	bIsRunningBack = true;
	CurrentAttackIndex = 0;
	
	//��������HP
	EnemyPawn->CurrentHealth=EnemyPawn->TotalHealth;
	EnemyPawn->UpdateHealthBar();

	GetWorldTimerManager().ClearTimer(TimerHandle_Patrol);
	GetWorldTimerManager().ClearTimer(TimerHandle_OnAnimPlayOver);
	GetWorldTimerManager().ClearTimer(TimerHandle_CalDis);
	TargetActor = nullptr;
	bWasAggroed = false;
	MoveToLocation(EnemyPawn->StartLocation);

}

void ANormalEnmeyController::Patrol()
{
	const float SearchRadius = 1000.0f;
	if (NavSys)
	{
		FNavLocation RandomPt;
		//����λ�� �뾶 ���ص�λ��
		bool bFound = NavSys->GetRandomPointInNavigableRadius(EnemyPawn->GetActorLocation(), SearchRadius, RandomPt);
		if (bFound)
		{
			MoveToLocation(RandomPt.Location);
		}
	}
}

