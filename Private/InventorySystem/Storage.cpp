// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Storage.h"
#include "Character/RBaseCharacter.h"
#include "InventorySystem/BaseItem.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget/Inventory/StorageSlotWidget.h"
#include "UserWidget/Inventory/StorageWidget.h"
#include "InventorySystem/Inventory.h"
#include "SaveGames/StorageSave.h"
#include "NPC/StorageKeeper.h"

// Sets default values
AStorage::AStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStorage::BeginPlay()
{
	Super::BeginPlay();

	 PlayerCharacter= Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	 
	 if(PlayerCharacter)
	 {
		StorageWidget= PlayerCharacter->MainUserWidget->StorageWidget;
		UE_LOG(LogTemp, Warning, TEXT("AStorage::BeginPlay()"));
	 }

	 /* if(UGameplayStatics::DoesSaveGameExist(SavedSlotName,0))
	  {
		 LoadStorage();
	  }*/

	 Slots.SetNum(AmountOfSlots);

	
}

// Called every frame
void AStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AStorage::BIsSlotEmpty(int Index)
{
	if (Slots[Index].ItemClass)
	{	//�ж�������û������ �оͲ�Ϊ��
		return false;
	}
	else
	{
		return true;
	}
}

class ABaseItem* AStorage::GetItemAtIndex(int Index)
{
	if (!BIsSlotEmpty(Index))
	{
		return Slots[Index].ItemClass->GetDefaultObject<ABaseItem>();
	}
	else
	{
		return nullptr;
	}
}

int AStorage::SearchEmptySlot()
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		//û����Ʒ��Ϊ��
		if (!Slots[i].ItemClass)
		{
			return i;
		}
	}

	//�����ķ���ֵ ������һ�� ˵��û�ҿղ��
	return Slots.Num();

}

int AStorage::SearchFreeStack(TSubclassOf<class ABaseItem> Item)
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i].ItemClass && Slots[i].ItemClass == Item && Slots[i].Amout < MaxStackSisze)
		{
			return i;
		}
	}
	return Slots.Num();
}

int AStorage::AddItem(TSubclassOf<class ABaseItem>Item, int Amount)
{

	int FoundIndex = 0;

	//�ж���Ʒ�ǲ��ǿ��Զѻ�
	//if (Cast<ABaseItem>(Item)->ItemInfo.bCanBeStacked)
	if (Item->GetDefaultObject<ABaseItem>()->ItemInfo.bCanBeStacked)
	{
		//�ܶѻ� Ѱ�Ҳ��

		//û�ҵ���ͬ��
		if (SearchFreeStack(Item) == Slots.Num())
		{
			//�п��ڲ��
			if (SearchEmptySlot() != Slots.Num())
			{

				FoundIndex = SearchEmptySlot();
				//���еĴ��� ���ѷ�����
				if (Amount > MaxStackSisze)
				{

					Slots[FoundIndex] = FInventorySlot{ Item ,MaxStackSisze };
				
					UpdateSlotByIndex(FoundIndex); //���µ����Լ����
					//UpdateCarftingMenu();//���ºϳ�ui
					return AddItem(Item, Amount - MaxStackSisze);
				}
				else
				{
					Slots[FoundIndex] = FInventorySlot{ Item ,Amount };

				

					UpdateSlotByIndex(FoundIndex); //���µ����Լ����
					//UpdateCarftingMenu();//���ºϳ�ui
					return 0;
				}
			}
			else
			{
				return Amount;
			}
		}
		else
		{

			FoundIndex = SearchFreeStack(Item);
			//�ҵ���ͬ�Ĳ�� �ܷŵĿռ䲻��
			if ((Slots[FoundIndex].Amout + Amount) > MaxStackSisze)
			{
				//����� ֻ�ܷ�������
				int Temp = Slots[FoundIndex].Amout + Amount - MaxStackSisze;
				Slots[FoundIndex] = FInventorySlot{ Item,MaxStackSisze };

				

				UpdateSlotByIndex(FoundIndex);
				//UpdateCarftingMenu();//���ºϳ�ui
				return AddItem(Item, Temp);
			}
			else  //����һ�η���
			{
				//ԭ�е�+�����ڵ�
				Slots[FoundIndex] = FInventorySlot{ Item,(Slots[FoundIndex].Amout + Amount) };
			
				UpdateSlotByIndex(FoundIndex);
				//UpdateCarftingMenu();//���ºϳ�ui
				return 0;
			}

		}

	}
	else
	{
		//���ܶѻ����ҿյĲ�� 1��1���ķ�
		if (SearchEmptySlot() != Slots.Num())
		{
			FoundIndex = SearchEmptySlot();//�ҵ��յĲ�� �����Ǳ��������Ĳ�� �пյ������������

			//								��� ��ŵ�����									
			Slots[FoundIndex] = FInventorySlot{ Item,1 };

	
			UpdateSlotByIndex(FoundIndex);
			//UpdateCarftingMenu();//���ºϳ�ui
			if (Amount > 1)
			{
				//�ݹ� ûһ�˼�1����
				return AddItem(Item, Amount - 1);
			}
			else
			{
				return 0;
			}

		}
		else
		{
			return Amount;
		}
	}
}



void AStorage::UpdateSlotByIndex(int Index)
{
	StorageWidget->StorageSlotWidgets[Index]->UpdateSlot();
}

int AStorage::GetAmountAtIndex(int Index)
{
	return Slots[Index].Amout;
}

bool AStorage::SwapSlot(int Index1, int Index2)
{
	if (Index1 > Slots.Num() || Index2 > Slots.Num())
	{
		return false;
	}
	else
	{
		FInventorySlot TempSlot = Slots[Index1];
		Slots[Index1] = Slots[Index2];
		Slots[Index2] = TempSlot;
		UpdateSlotByIndex(Index1);
		UpdateSlotByIndex(Index2);
		return true;
	}
}

bool AStorage::AddToInedx(int FromIndex, int ToIndex)
{
	if (Slots[FromIndex].ItemClass == Slots[ToIndex].ItemClass && GetItemAtIndex(FromIndex)->ItemInfo.bCanBeStacked)
	{
		if ((GetAmountAtIndex(FromIndex) + GetAmountAtIndex(ToIndex)) < MaxStackSisze)
		{
			Slots[ToIndex] = FInventorySlot{ Slots[FromIndex].ItemClass ,  GetAmountAtIndex(FromIndex) + GetAmountAtIndex(ToIndex) };
			Slots[FromIndex] = FInventorySlot{ nullptr,0 };
		}
		else
		{
			Slots[ToIndex] = FInventorySlot{ Slots[FromIndex].ItemClass ,  MaxStackSisze };
			Slots[FromIndex] = FInventorySlot{ Slots[FromIndex].ItemClass ,  GetAmountAtIndex(FromIndex) + GetAmountAtIndex(ToIndex) - MaxStackSisze };
		}

		UpdateSlotByIndex(ToIndex);
		UpdateSlotByIndex(FromIndex);
		return true;
	}
	else
	{
		return false;
	}
}


bool AStorage::RemoveItemAtIndex(int Index, int Amount)
{

	if (!BIsSlotEmpty(Index) && Amount > 0)
	{
		if (Amount >= GetAmountAtIndex(Index))
		{
			//ȫ������

			Slots[Index] = FInventorySlot{ nullptr,0 };
			UpdateSlotByIndex(Index);
			//UpdateCarftingMenu();//���ºϳ�ui
			return true;
		}
		else
		{
			Slots[Index] = FInventorySlot{ Slots[Index].ItemClass ,GetAmountAtIndex(Index) - Amount };
			
			UpdateSlotByIndex(Index);
			//UpdateCarftingMenu();//���ºϳ�ui
			return true;
		}
	}
	else
	{
		return false;
	}
}


bool AStorage::AddItemToIndex(int Index, TSubclassOf<ABaseItem>Item, int Amount)
{
	if (BIsSlotEmpty(Index) && Amount < MaxStackSisze)
	{
		Slots[Index] = FInventorySlot{ Item,Amount };
		UpdateSlotByIndex(Index);
		return true;
	}
	else
	{
		return false;
	}
}

bool AStorage::IncreaseAmountAtIndex(int Index, int Amount)
{
	if (!BIsSlotEmpty(Index) && (GetAmountAtIndex(Index) + Amount) <= MaxStackSisze)
	{
		Slots[Index].Amout += Amount;
		UpdateSlotByIndex(Index);
		return true;
	}
	else
	{
		return false;
	}
}

void AStorage::OpenStorage()
{
	bStorageOpen=true;
	StorageWidget->SetVisibility(ESlateVisibility::Visible);
}

void AStorage::CloseStorage()
{
	if (StorageWidget)
	{
		bStorageOpen = false;
		StorageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

bool AStorage::MoveFromInventoryToStorageByIndex(class AInventory* Inventory, int InventoryIndex, int StorageIndex)
{
	int AmountToAdd;

	if (BIsSlotEmpty(StorageIndex))
	{
		AmountToAdd=Inventory->GetAmountAtIndex(InventoryIndex);
		if(AddItemToIndex(StorageIndex,Inventory->Slots[InventoryIndex].ItemClass, AmountToAdd))
		{
			Inventory->RemoveItemAtIndex(InventoryIndex,AmountToAdd);
			return true;
		}
		else
		{
			return false;
		}
	} 
	else
	{
		if (Slots[StorageIndex].ItemClass == Inventory->Slots[InventoryIndex].ItemClass )
		{ 
			if (Slots[StorageIndex].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.bCanBeStacked && GetAmountAtIndex(StorageIndex) < MaxStackSisze)
			{
								//ʣ��ռ�
				AmountToAdd= (MaxStackSisze-GetAmountAtIndex(StorageIndex) < Inventory->GetAmountAtIndex(InventoryIndex)  ) ?
					//�ܷż����Ǽ���                                   ���ܷ���
				( MaxStackSisze -GetAmountAtIndex(StorageIndex) ) : Inventory->GetAmountAtIndex(InventoryIndex);

				if (IncreaseAmountAtIndex(StorageIndex,AmountToAdd))
				{
					Inventory->RemoveItemAtIndex(InventoryIndex,AmountToAdd);
					return true;
				} 
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}

		} 
		else
		{
			FInventorySlot TempSlot;
			TempSlot= Slots[StorageIndex];
			Slots[StorageIndex]= Inventory->Slots[InventoryIndex];
			Inventory->Slots[InventoryIndex]=TempSlot;
			UpdateSlotByIndex(StorageIndex);
			Inventory->UpdateSlotByIndex(InventoryIndex);
			return true;
		}

	}
	
}

void AStorage::SaveStorage()
{
	if (StorageSaveInstance == nullptr)
	{
		StorageSaveInstance = Cast<UStorageSave>(UGameplayStatics::CreateSaveGameObject(UStorageSave::StaticClass()));
	}
	StorageSaveInstance->Slots= Slots;

	UGameplayStatics::SaveGameToSlot(StorageSaveInstance, SavedSlotName,0);
}

void AStorage::LoadStorage()
{
	if (!StorageSaveInstance)
	{
		StorageSaveInstance= Cast<UStorageSave>(UGameplayStatics::LoadGameFromSlot(SavedSlotName, 0));
	}

	Slots= StorageSaveInstance->Slots;





	StorageWidget->UpdateThisSlots();

}

