// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryEnum.h"
#include "InventoryStruct.generated.h"


USTRUCT()
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	TSubclassOf<class ABaseItem> ItemClass;
	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	int Amout;

};

USTRUCT()
struct FItemInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,Category="ItemInfo")
	FText Name;
	
	//描述说明
	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	FText Description;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	class UTexture2D* Icon;

	//可不可使用
	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	bool bCanBeUsed=true;

	//使用说明
	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	FText UseText;

	//该物品是不是不可以堆积
	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	bool bCanBeStacked;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	EItemCategories Category=EItemCategories::Consumle;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	FString CompareName;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	float Weifht;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	TArray<FInventorySlot>Recipe;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	int Price;
};

//用于存储
USTRUCT()
struct FSavedPickup
{
	GENERATED_BODY()
	//场景中所有需要保存的物体的ID，同一个类不同物品ID不同  不保存ID -1

	UPROPERTY()
	int ID;
	UPROPERTY()
	int RestAmount;
};


UCLASS()
class RPGPROJECT_API UInventoryStruct : public UObject
{
	GENERATED_BODY()
	
};
