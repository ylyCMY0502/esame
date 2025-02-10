// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageArea.generated.h"

UCLASS()
class RPGPROJECT_API ADamageArea : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ADamageArea();

	class ARBaseCharacter* Player;

	UPROPERTY(VisibleAnywhere,Category="Component")
	class UParticleSystemComponent* ParticleSystem;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	class UBoxComponent* BoxComponent;

	FTimerHandle TimerHandle_DamageArea;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//�����룻Ϳ����ʩ�ӣ�ʵʩ��Ӧ�ã����ã��ȣ�ɲ���������ã��ʺϣ�ָ�ƣ� �˺�
	void ApplayAreaDamage();

};
