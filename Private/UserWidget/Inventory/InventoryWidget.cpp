// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/InventoryWidget.h"
#include "Components/WrapBox.h"
#include "../../../Public/UserWidget/Inventory/InventorySlotWidget.h"
#include "../../../Public/InventorySystem/Inventory.h"
#include "UMG/Public/Components/Button.h"
#include "Character/RBaseCharacter.h"
#include "UserWidget/MainUserWidget.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "UserWidget/Inventory/InventoryDragDropOperation.h"
#include "Other/StaticLibrary.h"
#include "UMG/Public/Components/ComboBoxString.h"
#include "UMG/Public/Components/TextBlock.h"





void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this,&UInventoryWidget::OnCloseButtonClicke);
	SortButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnSortButtonClicked);


	for (ESortType Type :UStaticLibrary::EnumGetList<ESortType>(TEXT("ESortType")))
	{
		
		SortCategoryBox->AddOption(UStaticLibrary::GetEnumValueAsString<ESortType>(TEXT("ESortType"), Type));
	}
	SortCategoryBox->SetSelectedOption(SortCategoryBox->GetOptionAtIndex(0));


}

void UInventoryWidget::OnCloseButtonClicke()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	InventoryRef->PlayerCharacter->MainUserWidget->bInventoryVisibling=false; 
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::RightMouseButton).NativeReply;
	}
	else
	{
		return FReply::Handled();
	}
}

void UInventoryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected( InGeometry,InMouseEvent, OutOperation);
	UDragDropOperation* TempDrag=UWidgetBlueprintLibrary::CreateDragDropOperation(InventoryDragDropOperation);
	//ÍÏ×§ÏÔÊ¾
	TempDrag->DefaultDragVisual=this;
	TempDrag->Payload=this;
	TempDrag->Pivot=EDragPivot::MouseDown;
	OutOperation= TempDrag;

	if(Cast<UInventoryDragDropOperation>(OutOperation))
	{
		Cast<UInventoryDragDropOperation>(OutOperation)->WidgetToDrag=this;
		Cast<UInventoryDragDropOperation>(OutOperation)->OffsetPos =InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		
	}
	this->RemoveFromParent();
}



void UInventoryWidget::UpdateWeight(float CurrentWeight, float TotalWeight)
{
	CurrentlWeightText->SetText(FText::AsNumber(CurrentWeight));
	TotalWeightText->SetText(FText::AsNumber(TotalWeight));

	if (CurrentWeight > TotalWeight)
	{
		CurrentlWeightText->SetColorAndOpacity(FLinearColor(0.3,0,0,1));
		CurrentlWeightTextG->SetColorAndOpacity(FLinearColor(0.3, 0, 0, 1));
	} 
	else
	{
		CurrentlWeightText->SetColorAndOpacity(FLinearColor(0.5,0.3,0.2,0.6));
		CurrentlWeightTextG->SetColorAndOpacity(FLinearColor(0.5, 0.3, 0.2, 0.6));
	}
}



void UInventoryWidget::GenerateSlotWidget()
{
	
	InventoryBox->ClearChildren();
	InventorySlotWidgets.Empty();
	

	for (int i=0;i<InventoryRef->Slots.Num();i++)
	{
		UInventorySlotWidget* InventorySlotWidget=CreateWidget<UInventorySlotWidget>(GetWorld(),LoadClass<UInventorySlotWidget>(GetWorld(),TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_InventroySlot.WBP_InventroySlot_C'")));
		InventorySlotWidget->InventoryRef=InventoryRef;
		InventorySlotWidget->SlotIndex=i;
		InventorySlotWidget->UpdateSlot();
		InventoryBox->AddChildToWrapBox(InventorySlotWidget);
		InventorySlotWidgets.Add(InventorySlotWidget);
	}
}

void UInventoryWidget::OnSortButtonClicked()
{
	/*if (bReversed)
	{
		InventoryRef->SortInventory(static_cast<ESortType>(SortCategoryBox->FindOptionIndex(SortCategoryBox->GetSelectedOption())), true);
		bReversed=false;
	}
	else
	{
		InventoryRef->SortInventory(static_cast<ESortType>(SortCategoryBox->FindOptionIndex(SortCategoryBox->GetSelectedOption())), false);
		bReversed = true;
	}*/
	
	InventoryRef->SortInventory(static_cast<ESortType>(SortCategoryBox->FindOptionIndex(SortCategoryBox->GetSelectedOption())), bReversed);
	bReversed=!bReversed;
}

void UInventoryWidget::UpdateCoinText(int Coin)
{
	CoinText->SetText(FText::AsNumber(Coin));
}

void UInventoryWidget::UpdateThisSlots()
{
	for (int i = 0; i < InventoryRef->Slots.Num(); i++)
	{
		InventorySlotWidgets[i]->UpdateSlot();
	}
}
