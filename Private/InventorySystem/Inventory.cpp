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


	//--------------加载------------
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

	//设置插槽数量
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
	{	//判断上面有没有物体 有就不为空
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
		//没有物品就为空
		if (! Slots[i].ItemClass)
		{
			return i;
		}
	}

	//函数的返回值 与数组一样 说明没找空插槽
	return Slots.Num();
}

//返回的是第几个  找不到就返回最大值  /例如 0123 返回4/
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

	//判断物品是不是可以堆积
	//if (Cast<ABaseItem>(Item)->ItemInfo.bCanBeStacked)
	if (Item->GetDefaultObject<ABaseItem>()->ItemInfo.bCanBeStacked)
	{
		//能堆积 寻找插槽

		//没找到相同的
		if (SearchFreeStack(Item) == Slots.Num())
		{
			//有空在插槽
			if (SearchEmptySlot() !=Slots.Num())
			{

				FoundIndex = SearchEmptySlot();
				//现有的大于 最大堆放数量
				if (Amount > MaxStackSisze)
				{
					
					Slots[FoundIndex] = FInventorySlot{Item ,MaxStackSisze};
					//添加重量
					AddWeughtForItem(Item, MaxStackSisze);
					UpdateSlotByIndex(FoundIndex); //更新单个自己插槽
					UpdateCarftingMenu();//更新合成ui
					return AddItem(Item, Amount - MaxStackSisze);
				}
				else
				{
					Slots[FoundIndex] = FInventorySlot{ Item ,Amount };

					//添加重量
					AddWeughtForItem(Item, Amount);

					UpdateSlotByIndex(FoundIndex); //更新单个自己插槽
					UpdateCarftingMenu();//更新合成ui
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
			//找到相同的插槽 能放的空间不足
			if ( (Slots[FoundIndex].Amout+Amount)> MaxStackSisze )
			{
				//多余的 只能放入最大的
				int Temp= Slots[FoundIndex].Amout + Amount - MaxStackSisze;
				Slots[FoundIndex] = FInventorySlot{Item,MaxStackSisze};

				AddWeughtForItem(Item, MaxStackSisze);

				UpdateSlotByIndex(FoundIndex);
				UpdateCarftingMenu();//更新合成ui
				return AddItem(Item, Temp);
			} 
			else  //可以一次放下
			{
				//原有的+上现在的
				Slots[FoundIndex] = FInventorySlot{ Item,(Slots[FoundIndex].Amout + Amount) };
				AddWeughtForItem(Item, (Slots[FoundIndex].Amout + Amount));
				UpdateSlotByIndex(FoundIndex);
				UpdateCarftingMenu();//更新合成ui
				return 0;
			}

		}

	}
	else
	{
		//不能堆积，找空的插槽 1个1个的放
		if (SearchEmptySlot() !=Slots.Num())
		{
			FoundIndex=SearchEmptySlot();//找到空的插槽 函数是遍历索引的插槽 有空的立马返回序号

		//								插槽 存放的数量									
			Slots[FoundIndex] = FInventorySlot{Item,1};

			AddWeughtForItem(Item, 1);
			UpdateSlotByIndex(FoundIndex);
			UpdateCarftingMenu();//更新合成ui
			if(Amount >1)
			{
				//递归 没一此加1个吧
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
			//全部都掉
			
			RmoveWeightForItem(Slots[Index].ItemClass, GetAmountAtIndex(Index));
			Slots[Index] = FInventorySlot{ nullptr,0 };
			UpdateSlotByIndex(Index);
			UpdateCarftingMenu();//更新合成ui
			return true;
		} 
		else
		{
			Slots[Index] = FInventorySlot{ Slots[Index].ItemClass ,GetAmountAtIndex(Index)- Amount };
			RmoveWeightForItem(Slots[Index].ItemClass, GetAmountAtIndex(Index) - Amount);
			UpdateSlotByIndex(Index);
			UpdateCarftingMenu();//更新合成ui
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
			//找到空的插槽
			if (FonudIndex !=Slots.Num())
			{
				//原来的插槽
				Slots[Index]=FInventorySlot{ Slots[Index].ItemClass, GetAmountAtIndex(Index)-Amount};
				//新分的插槽
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
//                                     主动的要换的
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
	if (FilterEmptySlot().Num()>1 ) //没有赋值 Slots 没有改变
	{
		//FilterEmptySlot() 先将物体全部排列在一起 去除空格
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
		//不为空
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
				//正反
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
				//加入进来的 是来至那一个空格的 记录下来
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
			//一个卡槽够了
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
				//剩余空间
				AmountToAdd = (MaxStackSisze - GetAmountAtIndex(InventoryIndex) < Storage->GetAmountAtIndex(StorageIndex)) ?
					//能放几个是几个                                   都能放下
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
			//找到快捷键  不是空  使用物品
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
