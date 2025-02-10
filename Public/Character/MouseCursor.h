// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MouseCursor.generated.h"

UCLASS()
class RPGPROJECT_API AMouseCursor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMouseCursor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//���������齨
	//UPROPERTY(VisibleAnywhere)
	//class USceneComponent* SursorSceneComponent;
	//
	//�������������ʾ
	UPROPERTY(VisibleAnywhere)
	class UDecalComponent* SursorToMouseDecalWorld;

	//�����ײ���BOX
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

	/*UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/
};
