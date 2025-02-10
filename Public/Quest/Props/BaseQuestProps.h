// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Interface/InterationInterface.h"
#include "BaseQuestProps.generated.h"

UCLASS()
class RPGPROJECT_API ABaseQuestProps : public AActor ,public IInterationInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseQuestProps();

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent*InteractionWidget;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* ParticleSystemCom;
	
	UPROPERTY(EditAnywhere,Category="TEXT")
	FText Name;

	UPROPERTY(EditAnywhere, Category = "TEXT")
	FText InteractionTFext;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnEnterPlayerRadius(ARBaseCharacter* Character) override;
	virtual void OnLeavePlayerRadius(ARBaseCharacter* Character) override;
	//°´¼ü½»»¥
	virtual void OnIteractWith(ARBaseCharacter* Character) override;

};
