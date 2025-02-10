// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/InterationInterface.h"
#include "BsaeNPC.generated.h"

UCLASS()
class RPGPROJECT_API ABsaeNPC : public ACharacter,public IInterationInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABsaeNPC();
	//UI
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* InterctionWidget;

	//对话
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* MessageWidgetCom;

	UPROPERTY(EditAnywhere)
	class UPaperSpriteComponent* QuestIcon;


	UPROPERTY(EditAnywhere,Category="Quest")
	FText Name;

	//当前NPC上的任务
	UPROPERTY(EditAnywhere, Category = "Quest")
	TSubclassOf<class ABaseQuest> MyQuset;

	UPROPERTY(EditAnywhere, Category = "Quest")
	bool bHasQuest;

	UPROPERTY(EditAnywhere, Category = "Quest")
	int NPCId;
	//是不是要谈话
	UPROPERTY(EditAnywhere, Category = "Quest")
	bool bCanTalkTo;

	UPROPERTY(EditAnywhere, Category = "Quest")
	FText Message;

	UPROPERTY(EditAnywhere, Category = "Quest")
	float DefaultDuration=5.0f;

	FTimerHandle TimeHandle_ResetMessage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Quest")
	class UMessageWidget* MessageWidget;
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	bool bInPlayRadius;

	void ResetMessage();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnEnterPlayerRadius(ARBaseCharacter* Character) override;
	virtual void OnLeavePlayerRadius(ARBaseCharacter* Character) override;
	//按键交互
	virtual void OnIteractWith(ARBaseCharacter* Character) override;

	//交谈
	void OnTalkWith(ARBaseCharacter* Character);
	void ShowMessage(FText Message,float DefaultDuration, ARBaseCharacter* Character);
};
