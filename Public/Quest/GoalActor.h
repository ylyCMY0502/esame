// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalActor.generated.h"

UCLASS()
class RPGPROJECT_API AGoalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoalActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* DefaultRootCom;

	//Ŀ���
	UPROPERTY(VisibleAnywhere)
	class UPaperSpriteComponent* MinMapIcon;
	//Ŀ������
	UPROPERTY(VisibleAnywhere)
	class UPaperSpriteComponent* MinMapAreaIcon;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void InitializeMinimapIcon(bool bUseRadius,float Radius, FLinearColor CricleColor );
};
