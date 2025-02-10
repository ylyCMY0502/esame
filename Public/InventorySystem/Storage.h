// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryStruct.h"
#include "Storage.generated.h"

UCLASS()
class RPGPROJECT_API AStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStorage();

protected:
	//��۵�����
	int AmountOfSlots = 56;
	//����������
	int MaxStackSisze = 99;





	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:



	TArray<FInventorySlot>Slots;

	class ARBaseCharacter* PlayerCharacter;
	class UStorageWidget* StorageWidget;
	bool bStorageOpen;

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
	int AddItem(TSubclassOf<class ABaseItem>Item, int Amount);
	//-------------------------------------------------------

	void UpdateSlotByIndex(int Index);

	//�����ʱ��ȡ����ϵ� ����
	int GetAmountAtIndex(int Index);



	//����
	UFUNCTION(BlueprintCallable)
	bool SwapSlot(int Index1, int Index2);
	bool AddToInedx(int FromIndex, int ToIndex);// ��Ʒ��ͬʱ����

	//������Ʒ
	UFUNCTION(BlueprintCallable)
	bool RemoveItemAtIndex(int Index, int Amount);





	//-----------********** jc û��*************-------
	/*void UpdateActionMenuPosition(class UInventorySlotWidget* Slot, bool Visibility);

	void SortInventory(ESortType Type, bool BReversed);

	TArray <FInventorySlot> FilterEmptySlot();

	TArray <FInventorySlot> BubbleSortArray(TArray <FInventorySlot> InArray, ESortType Type, bool BReversed);

	void UpdateCarftingMenu();*/

	//--------------------------------------------------------------------------------




public:


	//���ӵ������
	bool AddItemToIndex(int Index, TSubclassOf<ABaseItem>Item, int Amount);
	//���еĶ������  --����66�� ����33��
	bool IncreaseAmountAtIndex(int Index, int Amount);

	void OpenStorage();
	void CloseStorage();

	class AInventory* Inventory;
	bool MoveFromInventoryToStorageByIndex(class AInventory*Inventory, int InventoryIndex,int StorageIndex);

	class UStorageSave* StorageSaveInstance;

	FString SavedSlotName=TEXT("StorageSave");

	void SaveStorage();
	void LoadStorage();
};
