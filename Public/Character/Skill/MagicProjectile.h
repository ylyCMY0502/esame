// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicProjectile.generated.h"

UCLASS()
class RPGPROJECT_API AMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicProjectile();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ParticleSystemCom;
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollision;
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	//粒子系统-----------------------------
	//弹道
	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* MissileEffect;
	//碰撞后
	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ImpactEffect;

	//主技能
	class AMissileSkill* Skill;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
