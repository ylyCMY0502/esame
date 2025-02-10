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
	//��۵�����
	int AmountOfSlots=56;
	//����������
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

	//�ж��ǲ���Ϊ��
	bool BIsSlotEmpty(int Index);

	//�������� �õ�  �����Ʒ
	class ABaseItem* GetItemAtIndex(int  Index);


	//�����ķ���ֵ ������һ�� ˵��û�ҿղ��---------------------------------------------

	//  �ҿյĲ��       BaseItem ��Ϊ�շ���������ֵ
	int SearchEmptySlot();
	//��ͬ�Ĳ��
	int SearchFreeStack(TSubclassOf<class ABaseItem> Item);
	//�����ķ���ֵ ������һ�� ˵��û�ҿղ��---------------------------------------------

	//���
	int AddItem(TSubclassOf<class ABaseItem>Item , int Amount );
	//-------------------------------------------------------

	void UpdateSlotByIndex(int Index);

	//�����ʱ��ȡ����ϵ� ����
	int GetAmountAtIndex(int Index);

	//������Ʒ
	UFUNCTION(BlueprintCallable)
	bool RemoveItemAtIndex(int Index,int Amount);

	//����
	UFUNCTION(BlueprintCallable)
	bool SwapSlot(int Index1,int Index2);
	bool AddToInedx(int FromIndex, int ToIndex);


	//�ָ�
	UFUNCTION(BlueprintCallable)
	bool SplitStacke(int Index ,int Amount);
	bool SplitStackToInedx(int FromIndex, int ToIndex, int Amount);

	//ʹ��
	UFUNCTION(BlueprintCallable)
	void UseItemAtIndex(int Index);

	//�Ķ� bool
	void UpdateActionMenuPosition(class UInventorySlotWidget* Slot, bool Visibility);

	void SortInventory(ESortType Type, bool BReversed);
	
	TArray <FInventorySlot> FilterEmptySlot();

	TArray <FInventorySlot> BubbleSortArray(TArray <FInventorySlot> InArray, ESortType Type, bool BReversed);

	void AddWeught(float Weight);

	void RmoveWeight(float Weight);

	void AddWeughtForItem(TSubclassOf<ABaseItem>Item,int Amount);

	void RmoveWeightForItem(TSubclassOf<ABaseItem>Item, int Amount);

	//��ȡ �����Ʒ�ڰ�����������   ����ı��
	int GetToalAmountOfItem(TSubclassOf<ABaseItem>Item, TArray<int> &ItemIndices);
	//�����ǲ��ǹ���
	bool RemoveItem(TSubclassOf<ABaseItem>Item, int Amount);

	//��Щ��Ʒ�ǿ��Ա��ϳɵ�
	UPROPERTY(EditAnywhere,Category="Item")
	TArray<TSubclassOf<class ABaseItem> > CraftableItems;

	void UpdateCarftingMenu();


public:
	//������ID��   ÿ����Ʒֻ����һ��Id��������ͬ��BContainsID() �����ж� �ǲ���Ҫ�����  ����������
	TArray<FSavedPickup>lootedPickups;
	class UInventorySave* InventorySaveInstance;

	FString SaveSlotName=TEXT("InventorySave");


	void SaveInventory();
	
	bool BContainsID(int ID, int &Rest);

	void LoadPickups();

	void LoadInventory();


	//��ӵ������
	bool AddItemToIndex(int Index,TSubclassOf<ABaseItem>Item,int Amount);
	//���еĶ��� --����66�� ����33��
	bool IncreaseAmountAtIndex(int Index,int Amount);

	class AStorage* Storage;
	bool MoveFromStorageToInventoryByIndex(class AStorage* Storage, int StorageIndex, int InventoryIndex);

	// ���� ʹ�ü��ٺ�
	void UpdateHotkeyByIndex(int Inedx);
	//���
	void HandleItemHotkeyPress(FKey Key);

	void HandleSwapForHotkey(int Index1, int Index2);

};
