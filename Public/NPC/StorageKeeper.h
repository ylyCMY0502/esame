// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/BsaeNPC.h"
#include "StorageKeeper.generated.h"


class AStorage;
UCLASS()
class RPGPROJECT_API AStorageKeeper : public ABsaeNPC
{
	GENERATED_BODY()
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:

	
	AStorage* StorageRef;

	UPROPERTY(EditAnywhere,Category="Storage")
	TSubclassOf<AStorage>StorageClass;
	//°´¼ü½»»¥
	virtual void OnIteractWith(ARBaseCharacter* Character) override;

	virtual void OnLeavePlayerRadius(ARBaseCharacter* Character) override;
};
