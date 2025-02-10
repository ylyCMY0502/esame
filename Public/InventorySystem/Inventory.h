// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryStruct.h"
#include "Inventory.generated.h"

UCLASS()
class RPGPROJECT_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

protected:
	//插槽的数量
	int AmountOfSlots=56;
	//存放最大数量
	int MaxStackSisze=99;

	

	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	float CurrentWeight;

	bool bOverload;

	UPROPERTY(EditAnywhere)
	float TotalWeight=300;

	void UpdateWeight();


	TArray<FInventorySlot>Slots;

	class ARBaseCharacter* PlayerCharacter;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//判断是不是为空
	bool BIsSlotEmpty(int Index);

	//给出索引 得到  获得物品
	class ABaseItem* GetItemAtIndex(int  Index);


	//函数的返回值 与数组一样 说明没找空插槽---------------------------------------------

	//  找空的插槽       BaseItem 不为空返回索引的值
	int SearchEmptySlot();
	//相同的插槽
	int SearchFreeStack(TSubclassOf<class ABaseItem> Item);
	//函数的返回值 与数组一样 说明没找空插槽---------------------------------------------

	//存放
	int AddItem(TSubclassOf<class ABaseItem>Item , int Amount );
	//-------------------------------------------------------

	void UpdateSlotByIndex(int Index);

	//鼠标点击时获取序号上的 数量
	int GetAmountAtIndex(int Index);

	//丢弃物品
	UFUNCTION(BlueprintCallable)
	bool RemoveItemAtIndex(int Index,int Amount);

	//交换
	UFUNCTION(BlueprintCallable)
	bool SwapSlot(int Index1,int Index2);
	bool AddToInedx(int FromIndex, int ToIndex);


	//分割
	UFUNCTION(BlueprintCallable)
	bool SplitStacke(int Index ,int Amount);
	bool SplitStackToInedx(int FromIndex, int ToIndex, int Amount);

	//使用
	UFUNCTION(BlueprintCallable)
	void UseItemAtIndex(int Index);

	//改动 bool
	void UpdateActionMenuPosition(class UInventorySlotWidget* Slot, bool Visibility);

	void SortInventory(ESortType Type, bool BReversed);
	
	TArray <FInventorySlot> FilterEmptySlot();

	TArray <FInventorySlot> BubbleSortArray(TArray <FInventorySlot> InArray, ESortType Type, bool BReversed);

	void AddWeught(float Weight);

	void RmoveWeight(float Weight);

	void AddWeughtForItem(TSubclassOf<ABaseItem>Item,int Amount);

	void RmoveWeightForItem(TSubclassOf<ABaseItem>Item, int Amount);

	//获取 这个物品在包裹的总数量   数组的编号
	int GetToalAmountOfItem(TSubclassOf<ABaseItem>Item, TArray<int> &ItemIndices);
	//材料是不是够的
	bool RemoveItem(TSubclassOf<ABaseItem>Item, int Amount);

	//那些物品是可以被合成的
	UPROPERTY(EditAnywhere,Category="Item")
	TArray<TSubclassOf<class ABaseItem> > CraftableItems;

	void UpdateCarftingMenu();


public:
	//用与存放ID的   每个物品只能有一个Id不必能相同，BContainsID() 用于判断 是不是要保存的  保存后的数量
	TArray<FSavedPickup>lootedPickups;
	class UInventorySave* InventorySaveInstance;

	FString SaveSlotName=TEXT("InventorySave");


	void SaveInventory();
	
	bool BContainsID(int ID, int &Rest);

	void LoadPickups();

	void LoadInventory();


	//添加到插槽上
	bool AddItemToIndex(int Index,TSubclassOf<ABaseItem>Item,int Amount);
	//已有的东西 --比如66个 还差33个
	bool IncreaseAmountAtIndex(int Index,int Amount);

	class AStorage* Storage;
	bool MoveFromStorageToInventoryByIndex(class AStorage* Storage, int StorageIndex, int InventoryIndex);

	// 比如 使用减少后
	void UpdateHotkeyByIndex(int Inedx);
	//点击
	void HandleItemHotkeyPress(FKey Key);

	void HandleSwapForHotkey(int Index1, int Index2);

};
