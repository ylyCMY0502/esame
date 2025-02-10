// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/MagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/Skill/MissileSkill.h"
#include "Character/RBaseCharacter.h"
#include "Enemy/NormalEnemy.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/DamageableInterface.h"


// Sets default values
AMagicProjectile::AMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AMagicProjectile::OnBeginOverlap);

	ParticleSystemCom = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystemCom->SetupAttachment(RootComponent);
	//不能放到组件上
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	//ProjectileMovementComponent->SetupAttachment(RootComponent);
	ProjectileMovementComponent->ProjectileGravityScale = 0;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	
}

// Called when the game starts or when spawned
void AMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	//AMissileSkill转过来的 //那个技能的拥有者  FActorSpawnParameters Params;  Params.Owner = this
	Skill = Cast<AMissileSkill>(GetOwner());
	MissileEffect = Skill->MissileEffect;
	ImpactEffect = Skill->ImpactEffect;
	ParticleSystemCom->SetTemplate(MissileEffect);
	//弹道的初始速度=设置的
	//ProjectileMovementComponent->InitialSpeed = Skill->GetCurrentStage().MissileSpeed;
	ProjectileMovementComponent->bIsHomingProjectile = true;
	//弹道的目标设置为 ANormalEnemy上  UArrowComponent* HitArrowCom
	if (Skill->GetPlayerReference()->SelectedEnemy)
	{
		ProjectileMovementComponent->HomingTargetComponent = Skill->GetPlayerReference()->SelectedEnemy->HitArrowCom;
	
		ProjectileMovementComponent->HomingTargetComponent = Skill->GetPlayerReference()->SelectedEnemy->HitArrowCom;
		ProjectileMovementComponent->HomingAccelerationMagnitude= Skill->GetCurrentStage().MissileSpeed;
	}
}


void AMagicProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT(".........qqqqqqqqqqqq............."));
	ANormalEnemy* NormalEnemy = Cast<ANormalEnemy>(OtherActor);
	if (NormalEnemy )
	{
		
		if (NormalEnemy->GetCapsuleComponent()== OtherComp)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, FVector(SweepResult.ImpactPoint));


			IDamageableInterface* DamageableInterface=Cast<IDamageableInterface>(NormalEnemy);
			if (DamageableInterface)
			{
				DamageableInterface->OnReceiveDamage(Skill->GetCurrentStage().Damage, Skill->GetCurrentStage().DamageType, Skill->GetSkillInfo().Element, Skill->GetCurrentStage().CriticalChance, Skill->GetPlayerReference(),Skill);
			}

			//清除本class
			Destroy();
		}

	}
}

// Called every frame
void AMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Skill)
	{
		if (Skill->GetPlayerReference())
		{
			if (Skill->GetPlayerReference()->SelectedEnemy)
			{
				if (Skill->GetPlayerReference()->SelectedEnemy->GetBDead())
				{
					Destroy();
				}
			}
		
		
		}
	}

	

}

