// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InterationInterface.h"
#include "InventoryStruct.h"
#include "BaseItem.generated.h"

UCLASS()
class RPGPROJECT_API ABaseItem : public AActor,public IInterationInterface
{
	GENERATED_BODY()
	

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComp;
	

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent*InteractionWidgetComp;
	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ParticleSystemComp;


	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	FText Name;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	FText InteractionTFext;


public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollisonComp;


	UPROPERTY(EditAnywhere,Category="ItemInfo")
	FItemInfo ItemInfo;
	
	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	int Amount=1;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	int ID=-1;

	int Index;
	class AInventory* InventoryRef;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void OnEnterPlayerRadius(ARBaseCharacter* Character) override;
	virtual void OnLeavePlayerRadius(ARBaseCharacter* Character) override;
	//°´¼ü½»»¥
	virtual void OnIteractWith(ARBaseCharacter* Character) override;


	virtual void OnUsed();

};
