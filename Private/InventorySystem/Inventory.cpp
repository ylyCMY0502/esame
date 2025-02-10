// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Inventory.h"
#include "InventorySystem/BaseItem.h"
#include "Character/RBaseCharacter.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "UserWidget/Inventory/ActionMenuWidget.h"
#include "UObject/NoExportTypes.h"
#include "Components/BoxComponent.h"
#include "UserWidget/Inventory/CraftingMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGames/InventorySave.h"
#include "InventorySystem/Storage.h"
#include "UserWidget/Inventory/ItemHotkeyWidget.h"

// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = Cast<ARBaseCharacter>(GetOwner());


	//--------------����------------
	/*if (UGameplayStatics::DoesSaveGameExist(SaveSlotName,0))
	{
		LoadInventory();
		if(CurrentWeight>TotalWeight)
		{
			bOverload=true;
			PlayerCharacter->OnOverloaded();
		}
	}*/
	//----------------------------

	//���ò������
	Slots.SetNum(AmountOfSlots);
	
}

void AInventory::UpdateWeight()
{
	PlayerCharacter->MainUserWidget->InventoryWidget->UpdateWeight(CurrentWeight,TotalWeight);
}

// Called every frame
void AInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AInventory::BIsSlotEmpty(int Index)
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

class ABaseItem* AInventory::GetItemAtIndex(int Index)
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

int AInventory::SearchEmptySlot()
{
	for (int i=0 ;i<Slots.Num();i++)
	{
		//û����Ʒ��Ϊ��
		if (! Slots[i].ItemClass)
		{
			return i;
		}
	}

	//�����ķ���ֵ ������һ�� ˵��û�ҿղ��
	return Slots.Num();
}

//���ص��ǵڼ���  �Ҳ����ͷ������ֵ  /���� 0123 ����4/
int AInventory::SearchFreeStack(TSubclassOf<class ABaseItem> Item)
{
	UE_LOG(LogTemp, Warning, TEXT("%d ----SearchFreeStack---- "), Slots.Num());
	
	for (int i=0;i<Slots.Num();i++)
	{
		if(Slots[i].ItemClass  && Slots[i].ItemClass==Item  && Slots[i].Amout<MaxStackSisze)
		{
			return i;
		}
	}
	return Slots.Num();
}

int AInventory::AddItem(TSubclassOf<class ABaseItem>Item, int Amount)
{

	int FoundIndex=0;

	//�ж���Ʒ�ǲ��ǿ��Զѻ�
	//if (Cast<ABaseItem>(Item)->ItemInfo.bCanBeStacked)
	if (Item->GetDefaultObject<ABaseItem>()->ItemInfo.bCanBeStacked)
	{
		//�ܶѻ� Ѱ�Ҳ��

		//û�ҵ���ͬ��
		if (SearchFreeStack(Item) == Slots.Num())
		{
			//�п��ڲ��
			if (SearchEmptySlot() !=Slots.Num())
			{

				FoundIndex = SearchEmptySlot();
				//���еĴ��� ���ѷ�����
				if (Amount > MaxStackSisze)
				{
					
					Slots[FoundIndex] = FInventorySlot{Item ,MaxStackSisze};
					//�������
					AddWeughtForItem(Item, MaxStackSisze);
					UpdateSlotByIndex(FoundIndex); //���µ����Լ����
					UpdateCarftingMenu();//���ºϳ�ui
					return AddItem(Item, Amount - MaxStackSisze);
				}
				else
				{
					Slots[FoundIndex] = FInventorySlot{ Item ,Amount };

					//�������
					AddWeughtForItem(Item, Amount);

					UpdateSlotByIndex(FoundIndex); //���µ����Լ����
					UpdateCarftingMenu();//���ºϳ�ui
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

			FoundIndex= SearchFreeStack(Item);
			//�ҵ���ͬ�Ĳ�� �ܷŵĿռ䲻��
			if ( (Slots[FoundIndex].Amout+Amount)> MaxStackSisze )
			{
				//����� ֻ�ܷ�������
				int Temp= Slots[FoundIndex].Amout + Amount - MaxStackSisze;
				Slots[FoundIndex] = FInventorySlot{Item,MaxStackSisze};

				AddWeughtForItem(Item, MaxStackSisze);

				UpdateSlotByIndex(FoundIndex);
				UpdateCarftingMenu();//���ºϳ�ui
				return AddItem(Item, Temp);
			} 
			else  //����һ�η���
			{
				//ԭ�е�+�����ڵ�
				Slots[FoundIndex] = FInventorySlot{ Item,(Slots[FoundIndex].Amout + Amount) };
				AddWeughtForItem(Item, (Slots[FoundIndex].Amout + Amount));
				UpdateSlotByIndex(FoundIndex);
				UpdateCarftingMenu();//���ºϳ�ui
				return 0;
			}

		}

	}
	else
	{
		//���ܶѻ����ҿյĲ�� 1��1���ķ�
		if (SearchEmptySlot() !=Slots.Num())
		{
			FoundIndex=SearchEmptySlot();//�ҵ��յĲ�� �����Ǳ��������Ĳ�� �пյ����������

		//								��� ��ŵ�����									
			Slots[FoundIndex] = FInventorySlot{Item,1};

			AddWeughtForItem(Item, 1);
			UpdateSlotByIndex(FoundIndex);
			UpdateCarftingMenu();//���ºϳ�ui
			if(Amount >1)
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

void AInventory::UpdateSlotByIndex(int Index)
{
	
	PlayerCharacter->MainUserWidget->InventoryWidget->InventorySlotWidgets[Index]->UpdateSlot();
	UpdateHotkeyByIndex(Index);
}

int AInventory::GetAmountAtIndex(int Index)
{
	return Slots[Index].Amout;
}

bool AInventory::RemoveItemAtIndex(int Index, int Amount)
{
	if (!BIsSlotEmpty(Index) &&Amount>0)
	{
		if (Amount >= GetAmountAtIndex(Index))
		{
			//ȫ������
			
			RmoveWeightForItem(Slots[Index].ItemClass, GetAmountAtIndex(Index));
			Slots[Index] = FInventorySlot{ nullptr,0 };
			UpdateSlotByIndex(Index);
			UpdateCarftingMenu();//���ºϳ�ui
			return true;
		} 
		else
		{
			Slots[Index] = FInventorySlot{ Slots[Index].ItemClass ,GetAmountAtIndex(Index)- Amount };
			RmoveWeightForItem(Slots[Index].ItemClass, GetAmountAtIndex(Index) - Amount);
			UpdateSlotByIndex(Index);
			UpdateCarftingMenu();//���ºϳ�ui
			return true;
		}
	} 
	else
	{
		return false;
	}
}

bool AInventory::SwapSlot(int Index1, int Index2)
{
	if(Index1>Slots.Num() || Index2>Slots.Num() )
	{
		return false;
	}
	else
	{
		

		FInventorySlot TempSlot=Slots[Index1];
		Slots[Index1]= Slots[Index2];
		Slots[Index2]= TempSlot;
		HandleSwapForHotkey(Index1, Index2);

		UpdateSlotByIndex(Index1);
		UpdateSlotByIndex(Index2);

		//HandleSwapForHotkey(Index1, Index2);
		//UE_LOG(LogTemp,Warning,TEXT(" SwapSlot int1%d ,int2%d "),Index1,Index2);

		

		return true;
	}
}

bool AInventory::SplitStacke(int Index, int Amount)
{
	if (!BIsSlotEmpty(Index))
	{
		if (GetAmountAtIndex(Index)>Amount && GetItemAtIndex(Index)->ItemInfo.bCanBeStacked)
		{
			int FonudIndex=SearchEmptySlot();
			//�ҵ��յĲ��
			if (FonudIndex !=Slots.Num())
			{
				//ԭ���Ĳ��
				Slots[Index]=FInventorySlot{ Slots[Index].ItemClass, GetAmountAtIndex(Index)-Amount};
				//�·ֵĲ��
				Slots[FonudIndex]=FInventorySlot{ Slots[Index].ItemClass,Amount };
				UpdateSlotByIndex(Index);
				UpdateSlotByIndex(FonudIndex);

			

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
		return false;
	}
}
//                                     ������Ҫ����
bool AInventory::SplitStackToInedx(int FromIndex, int ToIndex, int Amount)
{
	if (!BIsSlotEmpty(FromIndex)  &&
		BIsSlotEmpty(ToIndex)     &&
		GetItemAtIndex(FromIndex)->ItemInfo.bCanBeStacked &&
		GetAmountAtIndex(FromIndex) > 1   &&
		GetAmountAtIndex(FromIndex)>Amount )
	{
		Slots[FromIndex] = FInventorySlot{ Slots[FromIndex].ItemClass , ( GetAmountAtIndex(FromIndex)- Amount) };
		Slots[ToIndex]= FInventorySlot{ Slots[FromIndex].ItemClass , Amount };
		
		UpdateSlotByIndex(ToIndex);
		UpdateSlotByIndex(FromIndex);
		return true;
	
	} 
	else
	{
		return false;
	}
}

void AInventory::UseItemAtIndex(int Index)
{
	if (Slots[Index].ItemClass)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABaseItem*Item=GetWorld()->SpawnActor<ABaseItem>(Slots[Index].ItemClass,Params);
		Item->InventoryRef=this;
		Item->Index=Index;

		if (Item->ItemInfo.Category==EItemCategories::Equipment)
		{
			Item->BoxCollisonComp->SetGenerateOverlapEvents(false);
		}
		Item->OnUsed();
	}
}

void AInventory::UpdateActionMenuPosition(class UInventorySlotWidget* Slot , bool Visibility)
{
	PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->Update(Slot->SlotIndex);

	int Y = Slot->SlotIndex / 7;
	int X = Slot->SlotIndex % 7;
	
	PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->SetRenderTranslation(FVector2D(72*X,72*Y));

	if ( Visibility )
	{
		PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PlayerCharacter->MainUserWidget->InventoryWidget->ActionMenu->SetVisibility(ESlateVisibility::Hidden);

	}

}
	

void AInventory::SortInventory(ESortType Type, bool BReversed)
{
	if (FilterEmptySlot().Num()>1 ) //û�и�ֵ Slots û�иı�
	{
		//FilterEmptySlot() �Ƚ�����ȫ��������һ�� ȥ���ո�
		Slots=BubbleSortArray(FilterEmptySlot(), Type, BReversed);
		Slots.SetNum(AmountOfSlots);
		for (int i =0; i<Slots.Num();i++)
		{
			UpdateSlotByIndex(i);
		}
	} 
	else
	{
		Slots= FilterEmptySlot();
		Slots.SetNum(AmountOfSlots);
		for (int i = 0; i < Slots.Num(); i++)
		{
			UpdateSlotByIndex(i);
		}
	}
}



TArray <FInventorySlot> AInventory::FilterEmptySlot()
{
	TArray<FInventorySlot> TempSlots;
	for (FInventorySlot Slot:Slots)
	{
		//��Ϊ��
		if (Slot.ItemClass)
		{
			TempSlots.Add(Slot);
		}
	}
	return TempSlots;
}




TArray <FInventorySlot> AInventory::BubbleSortArray(TArray <FInventorySlot> InArray, ESortType Type, bool BReversed)
{
	
	TArray <FInventorySlot> TempArray=InArray;
	for (int End= TempArray.Num()-1;End >0; End--)
	{
		for (int i=0; i<End;i++)
		{
			switch (Type)
			{
			case ESortType::Category:
				//����
				if (!BReversed) 
				{
					if (TempArray[i].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Category > TempArray[i + 1].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Category)
					{
						FInventorySlot TempSlot = TempArray[i];
						TempArray[i] = TempArray[i + 1];
						TempArray[i + 1] = TempSlot;
					}
				}
				else
				{
					if (TempArray[i].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Category < TempArray[i + 1].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.Category)
					{
						FInventorySlot TempSlot = TempArray[i];
						TempArray[i] = TempArray[i + 1];
						TempArray[i + 1] = TempSlot;
					}
				}
				break;

			case ESortType::Amount:

				if (!BReversed)
				{
					if (TempArray[i].Amout > TempArray[i + 1].Amout)
					{
						FInventorySlot TempSlot = TempArray[i];
						TempArray[i] = TempArray[i + 1];
						TempArray[i + 1] = TempSlot;

					}
				}
				else
				{
					if (TempArray[i].Amout < TempArray[i + 1].Amout)
					{
						FInventorySlot TempSlot = TempArray[i];
						TempArray[i] = TempArray[i + 1];
						TempArray[i + 1] = TempSlot;

					}
				}

				break;


			case ESortType::Name:

				if (!BReversed)
				{
					if (TempArray[i].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.CompareName.Compare( TempArray[i + 1].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.CompareName ) >0 )
					{
						FInventorySlot TempSlot = TempArray[i];
						TempArray[i] = TempArray[i + 1];
						TempArray[i + 1] = TempSlot;
					}
				}
				else
				{
					if (TempArray[i].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.CompareName.Compare(TempArray[i + 1].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.CompareName) < 0)
					{
						FInventorySlot TempSlot = TempArray[i];
						TempArray[i] = TempArray[i + 1];
						TempArray[i + 1] = TempSlot;
					}
					
				}
				break;
				
			default:
				break;
			}
		}
	}
	return TempArray;
}

void AInventory::AddWeught(float Weight)
{
	CurrentWeight +=Weight;
	UpdateWeight();

	if (! bOverload && CurrentWeight >TotalWeight)
	{
		bOverload=true;
		PlayerCharacter->OnOverloaded();
	}
}

void AInventory::RmoveWeight(float Weight)
{
	CurrentWeight -= Weight;
	UpdateWeight();

	if (bOverload && CurrentWeight <= TotalWeight)
	{
		bOverload = false;
		PlayerCharacter->OnOverloadedEnd();
	}

}

void AInventory::AddWeughtForItem(TSubclassOf<ABaseItem>Item, int Amount)
{
	AddWeught(Item->GetDefaultObject<ABaseItem>()->ItemInfo.Weifht*Amount);
}

void AInventory::RmoveWeightForItem(TSubclassOf<ABaseItem>Item, int Amount)
{
	RmoveWeight(Item->GetDefaultObject<ABaseItem>()->ItemInfo.Weifht * Amount);
}

int AInventory::GetToalAmountOfItem(TSubclassOf<ABaseItem>Item, TArray<int>& ItemIndices)
{
	int TempAmount=0;
	for (int i=0;i<Slots.Num();i++)
	{
		if (Slots[i].ItemClass)
		{
			if (Slots[i].ItemClass == Item)
			{
				TempAmount+=Slots[i].Amout;
				//��������� ��������һ���ո�� ��¼����
				ItemIndices.Add(i);
			}
		}
	}
	return TempAmount;
}




bool AInventory::RemoveItem(TSubclassOf<ABaseItem>Item, int Amount)
{
	TArray<int>ItemIndices;
	if(GetToalAmountOfItem(Item,ItemIndices)>=Amount)
	{
		for (int Index: ItemIndices)
		{
			//һ�����۹���
			if (GetAmountAtIndex(Index) >=Amount)
			{
				RemoveItemAtIndex(Index,Amount);
				break;
			} 
			else
			{
				Amount-= GetAmountAtIndex(Index);
				RemoveItemAtIndex(Index, GetAmountAtIndex(Index));
			}
		}
		return true;
	}
	else
	{
		return false;
	}

}



void AInventory::UpdateCarftingMenu()
{
	if(PlayerCharacter->MainUserWidget->CraftingMenu->CurrentItem)
	PlayerCharacter->MainUserWidget->CraftingMenu->UpdateDetailWindow(PlayerCharacter->MainUserWidget->CraftingMenu->CurrentItem);
}



void AInventory::SaveInventory()
{
	if(! InventorySaveInstance)
	{
		InventorySaveInstance=Cast<UInventorySave>(UGameplayStatics::CreateSaveGameObject(UInventorySave::StaticClass()));
	}

	InventorySaveInstance->SavedWeight=CurrentWeight;
	InventorySaveInstance->SavedSlots=Slots;
	InventorySaveInstance->SavedPickup=lootedPickups;

	UGameplayStatics::SaveGameToSlot(InventorySaveInstance,SaveSlotName,0);
}


bool AInventory::BContainsID(int ID, int& Rest)
{
	for (FSavedPickup Pickup:lootedPickups)
	{
		if(Pickup.ID== ID)
		{
			Rest=Pickup.RestAmount;
			return true;
		}
	}
	return false;
}

void AInventory::LoadPickups()
{
	TArray<AActor*>FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ABaseItem::StaticClass(),FoundActors);

	for (AActor* Actor: FoundActors)
	{
		ABaseItem *Item=Cast<ABaseItem>(Actor);
		if (Item)
		{
			if (Item->ID !=-1)
			{
				int Rest;
				if (BContainsID(Item->ID,Rest) )  //Rest &
				{
					if (Rest > 0)
					{
						Item->Amount =Rest;
					}
					else
					{
						Item->Destroy();
					}
				}
			}
		} 
		
	}
}

void AInventory::LoadInventory()
{
	if (!InventorySaveInstance)
	{
		InventorySaveInstance = Cast<UInventorySave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName,0));
	}

	CurrentWeight=InventorySaveInstance->SavedWeight ;
	PlayerCharacter->MainUserWidget->InventoryWidget->UpdateWeight(CurrentWeight, TotalWeight);

	//PlayerCharacter->MainUserWidget->InventoryWidget->UpdateCoinText(CurrentCoin);

	Slots=InventorySaveInstance->SavedSlots;
	PlayerCharacter->MainUserWidget->InventoryWidget->UpdateThisSlots();

	lootedPickups=InventorySaveInstance->SavedPickup;
	
	LoadPickups();

}




bool AInventory::AddItemToIndex(int Index, TSubclassOf<ABaseItem>Item, int Amount)
{
	if (BIsSlotEmpty(Index) && Amount <MaxStackSisze)
	{
		Slots[Index]=FInventorySlot{Item,Amount};
		AddWeughtForItem(Item,Amount);
		UpdateSlotByIndex(Index);
		return true;
	} 
	else
	{
		return false;
	}
}


bool AInventory::IncreaseAmountAtIndex(int Index, int Amount)
{
	if( ! BIsSlotEmpty(Index) &&  (GetAmountAtIndex(Index)+Amount) <= MaxStackSisze)
	{
		Slots[Index].Amout+=Amount;
		AddWeughtForItem(Slots[Index].ItemClass, Slots[Index].Amout);
		UpdateSlotByIndex(Index);
		return true;
	}
	else
	{
		return false;
	}
}

bool AInventory::MoveFromStorageToInventoryByIndex(class AStorage* Storage, int StorageIndex,int InventoryIndex )
{
	int AmountToAdd;

	if (BIsSlotEmpty(InventoryIndex))
	{
		AmountToAdd = Storage->GetAmountAtIndex(StorageIndex);
		if (AddItemToIndex(InventoryIndex, Storage->Slots[StorageIndex].ItemClass, AmountToAdd))
		{
			Storage->RemoveItemAtIndex(StorageIndex, AmountToAdd);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (Slots[InventoryIndex].ItemClass == Storage->Slots[StorageIndex].ItemClass )
		{ 
			if(Slots[InventoryIndex].ItemClass->GetDefaultObject<ABaseItem>()->ItemInfo.bCanBeStacked && GetAmountAtIndex(InventoryIndex) < MaxStackSisze)
				{
				//ʣ��ռ�
				AmountToAdd = (MaxStackSisze - GetAmountAtIndex(InventoryIndex) < Storage->GetAmountAtIndex(StorageIndex)) ?
					//�ܷż����Ǽ���                                   ���ܷ���
					(MaxStackSisze - GetAmountAtIndex(InventoryIndex)) : Storage->GetAmountAtIndex(StorageIndex);

				if (IncreaseAmountAtIndex(InventoryIndex, AmountToAdd))
				{
					Storage->RemoveItemAtIndex(StorageIndex, AmountToAdd);
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			FInventorySlot TempSlot;
			TempSlot = Slots[InventoryIndex];
			Slots[InventoryIndex] = Storage->Slots[StorageIndex];
			Storage->Slots[StorageIndex] = TempSlot;
			UpdateSlotByIndex(InventoryIndex);
			Storage->UpdateSlotByIndex(StorageIndex);
			
			return true;
		}

	}



}




void AInventory::UpdateHotkeyByIndex(int Index)
{
	for (UItemHotkeyWidget* HotKey : PlayerCharacter->MainUserWidget->ItemHotkeyWidgets )
	{
		if (! HotKey->bEmpty && HotKey->InvnetoryIndex == Index )
		{
		
			HotKey->UPdate();
		}
	}
}




void AInventory::HandleItemHotkeyPress(FKey Key)
{
	UE_LOG(LogTemp, Warning, TEXT("--HandleItemHotkeyPress--- "));
	for (UItemHotkeyWidget* HotKey : PlayerCharacter->MainUserWidget->ItemHotkeyWidgets)
	{
		
		if (HotKey->Key==Key && !HotKey->bEmpty )
		{
			UE_LOG(LogTemp, Warning, TEXT("----------HotKey->Key==Key && !HotKey->bEmpty---- "));
			//�ҵ���ݼ�  ���ǿ�  ʹ����Ʒ
			UseItemAtIndex(HotKey->InvnetoryIndex);
			break;
		}
	}

}

void AInventory::HandleSwapForHotkey(int Index1, int Index2)
{
	for (UItemHotkeyWidget* HotKey : PlayerCharacter->MainUserWidget->ItemHotkeyWidgets)
	{
		if ( !HotKey->bEmpty )
		{
			if (HotKey->InvnetoryIndex == Index1)
			{
				HotKey->InvnetoryIndex = Index2;
			}
			else if(HotKey->InvnetoryIndex == Index2)
			{
				HotKey->InvnetoryIndex = Index1;
			}

			
		} 
		
	}
}

bool AInventory::AddToInedx(int FromIndex, int ToIndex)
{
	if (Slots[FromIndex].ItemClass == Slots[ToIndex].ItemClass && GetItemAtIndex(FromIndex)->ItemInfo.bCanBeStacked)
	{
		if ( (GetAmountAtIndex(FromIndex)+GetAmountAtIndex(ToIndex)) < MaxStackSisze)
		{
			Slots[ToIndex]=FInventorySlot{Slots[FromIndex].ItemClass ,  GetAmountAtIndex(FromIndex)+GetAmountAtIndex(ToIndex)};
			Slots[FromIndex]=FInventorySlot{nullptr,0};
		} 
		else
		{
			Slots[ToIndex] = FInventorySlot{ Slots[FromIndex].ItemClass ,  MaxStackSisze };
			Slots[FromIndex] = FInventorySlot{ Slots[FromIndex].ItemClass ,  GetAmountAtIndex(FromIndex) + GetAmountAtIndex(ToIndex) - MaxStackSisze};
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
