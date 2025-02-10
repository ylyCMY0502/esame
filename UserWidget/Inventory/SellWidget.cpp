// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/SellWidget.h"

#include "../../../Public/InventorySystem/Inventory.h"
#include "../../../Public/InventorySystem/BaseItem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "../../../Public/UserWidget/MainUserWidget.h"
#include "../../../Public/Character/RBaseCharacter.h"
#include "../../../Public/UserWidget/Inventory/InventoryWidget.h"
#include "Components/WrapBox.h"
#include "Components/Image.h"
#include "UMG/Public/Components/EditableText.h"
#include "UserWidget/Inventory/ShopWidget.h"
#include "UMG/Public/Components/UniformGridPanel.h"



void USellWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MinusButton->OnClicked.AddDynamic(this, &USellWidget::OnMinusButtonClicked);
	MinusButton->OnPressed.AddDynamic(this, &USellWidget::OnMinusButtonPressed);
	MinusButton->OnReleased.AddDynamic(this, &USellWidget::OnMinusButtonReleased);

	PlusButton->OnClicked.AddDynamic(this, &USellWidget::OnPlusButtonClicked);
	PlusButton->OnPressed.AddDynamic(this, &USellWidget::OnPlusButtonPressed);
	PlusButton->OnReleased.AddDynamic(this, &USellWidget::OnPlusButtonReleased);


	ConfirmButton->OnClicked.AddDynamic(this, &USellWidget::OnConfirmButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &USellWidget::OnCancelButtonClicked);



}



void USellWidget::Update(int Index)
{
	CurrentIndex = Index;

	if (!InventoryRef->BIsSlotEmpty(CurrentIndex))
	{
		ItemInfo = InventoryRef->GetItemAtIndex(CurrentIndex)->ItemInfo;
		MaxAmout = InventoryRef->GetAmountAtIndex(CurrentIndex);
		NameText->SetText(ItemInfo.Name);
		NumText->SetText(FText::AsNumber(MaxAmout));
		ThrowCount = 1;
		CountEditableText->SetText(FText::AsNumber(ThrowCount));

		IconImage->SetBrushFromTexture(ItemInfo.Icon);

	}
	UPdatePrice();
}

void USellWidget::UPdatePrice()
{
	PriceText->SetText(FText::AsNumber(ThrowCount*ItemInfo.Price));
}

void USellWidget::IncreaseCount()
{
	ThrowCount = FCString::Atoi(*(CountEditableText->GetText().ToString()));
	ThrowCount = FMath::Clamp(ThrowCount + 1, 1, MaxAmout);
	CountEditableText->SetText(FText::AsNumber(ThrowCount));
	UPdatePrice();
}

void USellWidget::DecreaseCount()
{
	ThrowCount = FCString::Atoi(*(CountEditableText->GetText().ToString()));
	ThrowCount = FMath::Clamp(ThrowCount - 1, 1, MaxAmout);
	CountEditableText->SetText(FText::AsNumber(ThrowCount));
	UPdatePrice();
}

void USellWidget::OnMinusButtonClicked()
{
	ClickCount++;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CountToZero, this, &USellWidget::SetClickCountToZero, 0.3f, false);

	if (ClickCount > 1)
	{
		ClickCount = 0;
		ThrowCount = 1;
		CountEditableText->SetText(FText::AsNumber(ThrowCount));
		UPdatePrice();
	}
	else
	{
		DecreaseCount();
	}

}

void USellWidget::OnPlusButtonClicked()
{
	ClickCount++;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CountToZero, this, &USellWidget::SetClickCountToZero, 0.3f, false);

	if (ClickCount > 1)
	{
		ClickCount = 0;
		ThrowCount = MaxAmout;

		CountEditableText->SetText(FText::AsNumber(ThrowCount));
		//CountText->SetText(FText::AsNumber(ThrowCount));
		UPdatePrice();
	}
	else
	{
		IncreaseCount();
	}

}

void USellWidget::OnMinusButtonPressed()
{
	GetWorld()->GetTimerManager().SetTimer(TimeHanlde_Decrease, this, &USellWidget::DecreaseCount, 0.2f, true, 1.0f);
}

void USellWidget::OnPlusButtonPressed()
{
	GetWorld()->GetTimerManager().SetTimer(TimeHanlde_Increase, this, &USellWidget::IncreaseCount, 0.2f, true, 1.0f);
}

void USellWidget::OnMinusButtonReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeHanlde_Decrease);
}

void USellWidget::OnPlusButtonReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeHanlde_Increase);
}


void USellWidget::SetClickCountToZero()
{
	ClickCount = 0;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CountToZero);
}

void USellWidget::OnConfirmButtonClicked()
{
	//通过获取UI上输入的内容 得到FText类型 转FString 转 int
	ThrowCount = FCString::Atoi(*(CountEditableText->GetText().ToString()));
	if (InventoryRef->RemoveItemAtIndex(CurrentIndex, ThrowCount))
	{
		InventoryRef->PlayerCharacter->IncreaseCoin(ThrowCount*ItemInfo.Price);
		this->SetVisibility(ESlateVisibility::Hidden);
		InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(true);
		InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->CloseButton->SetIsEnabled(true);
		ShopWidget->GridPanel->SetIsEnabled(true);
		ShopWidget->UpdateAllItem();
	}
}

void USellWidget::OnCancelButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(true);
	ShopWidget->GridPanel->SetIsEnabled(true);
}
