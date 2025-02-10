// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget/Inventory/StorageWidget.h"
#include "Other/StaticLibrary.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/WrapBox.h"
#include "UserWidget/Inventory/StorageSlotWidget.h"
#include "InventorySystem/Storage.h"
#include "InventorySystem/Inventory.h"
#include "InventorySystem/BaseItem.h"

void UStorageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &UStorageWidget::OnCloseButtonClicke);
	//SortButton->OnClicked.AddDynamic(this, &UStorageWidget::OnSortButtonClicked);


	/*for (ESortType Type : UStaticLibrary::EnumGetList<ESortType>(TEXT("ESortType")))
	{

		SortCategoryBox->AddOption(UStaticLibrary::GetEnumValueAsString<ESortType>(TEXT("ESortType"), Type));
	}
	SortCategoryBox->SetSelectedOption(SortCategoryBox->GetOptionAtIndex(0));*/


}

void UStorageWidget::OnCloseButtonClicke()
{
	//this->SetVisibility(ESlateVisibility::Hidden);
	if(StorageRef)
	{ 
		StorageRef->CloseStorage();
	}
}

 

void UStorageWidget::GenerateSlotWidget(class AStorage* StorageRef)
{

	if(this->StorageRef ==nullptr)
	{
		this->StorageRef=StorageRef;
	}
	

	StorageBox->ClearChildren();
	StorageSlotWidgets.Empty();

	for (int i = 0; i < StorageRef->Slots.Num(); i++)
	{
		UStorageSlotWidget* StorageSlotWidget = CreateWidget<UStorageSlotWidget>(GetWorld(), LoadClass<UStorageSlotWidget>(GetWorld(), TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_StorageSlot.WBP_StorageSlot_C'")));
		StorageSlotWidget->StorageRef = StorageRef;
		StorageSlotWidget->SlotIndex = i;
		StorageSlotWidget->StorageWidget=this;
		StorageSlotWidget->UpdateSlot();
		StorageBox->AddChildToWrapBox(StorageSlotWidget);
		StorageSlotWidgets.Add(StorageSlotWidget);
	}
}




void UStorageWidget::UpdateThisSlots()
{
	for (int i = 0; i < StorageRef->Slots.Num(); i++)
	{
		StorageSlotWidgets[i]->UpdateSlot();
	}
	
}

void UStorageWidget::OnSlotClicked(int Index)
{

	int AmountToRemove =StorageRef->Slots[Index].Amout;
	int RestAmount =PlayerInventoryRef->AddItem(StorageRef->Slots[Index].ItemClass,AmountToRemove);
	if (RestAmount < AmountToRemove)
	{
		StorageRef->RemoveItemAtIndex(Index, AmountToRemove-RestAmount);
	}
}
