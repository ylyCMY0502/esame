// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ThrowWidget.h"
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



void UThrowWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MinusButton->OnClicked.AddDynamic(this,&UThrowWidget::OnMinusButtonClicked);
	MinusButton->OnPressed.AddDynamic(this, &UThrowWidget::OnMinusButtonPressed);
	MinusButton->OnReleased.AddDynamic(this, &UThrowWidget::OnMinusButtonReleased);

	PlusButton->OnClicked.AddDynamic(this,&UThrowWidget::OnPlusButtonClicked);
	PlusButton->OnPressed.AddDynamic(this, &UThrowWidget::OnPlusButtonPressed);
	PlusButton->OnReleased.AddDynamic(this, &UThrowWidget::OnPlusButtonReleased);


	ConfirmButton->OnClicked.AddDynamic(this, &UThrowWidget::OnConfirmButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UThrowWidget::OnCancelButtonClicked);

	

}



void UThrowWidget::Update(int Index)
{
	CurrentIndex=Index;

	if (!InventoryRef->BIsSlotEmpty(CurrentIndex))
	{
		ItemInfo=InventoryRef->GetItemAtIndex(CurrentIndex)->ItemInfo;
		MaxAmout= InventoryRef->GetAmountAtIndex(CurrentIndex);
		NameText->SetText(ItemInfo.Name);
		NumText->SetText(FText::AsNumber(MaxAmout));
		ThrowCount=1;
		CountEditableText->SetText(FText::AsNumber(ThrowCount));

		IconImage->SetBrushFromTexture(ItemInfo.Icon);

	} 

}

void UThrowWidget::IncreaseCount()
{
	ThrowCount = FCString::Atoi(*(CountEditableText->GetText().ToString()));
	ThrowCount = FMath::Clamp(ThrowCount + 1, 1, MaxAmout);
	CountEditableText->SetText(FText::AsNumber(ThrowCount));
}

void UThrowWidget::DecreaseCount()
{
	ThrowCount = FCString::Atoi(*(CountEditableText->GetText().ToString()));
	ThrowCount=FMath::Clamp(ThrowCount-1,1,MaxAmout);
	CountEditableText->SetText(FText::AsNumber(ThrowCount));
}

void UThrowWidget::OnMinusButtonClicked()
{
	ClickCount++;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CountToZero, this, &UThrowWidget::SetClickCountToZero, 0.3f, false);

	if (ClickCount>1)
	{
		ClickCount=0;
		ThrowCount=1;
		CountEditableText->SetText(FText::AsNumber(ThrowCount));
	} 
	else
	{
		DecreaseCount();
	}
	
}

void UThrowWidget::OnPlusButtonClicked()
{
	ClickCount++;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CountToZero, this, &UThrowWidget::SetClickCountToZero, 0.3f, false);

	if (ClickCount > 1)
	{
		ClickCount = 0;
		ThrowCount = MaxAmout;

		CountEditableText->SetText(FText::AsNumber(ThrowCount));
		//CountText->SetText(FText::AsNumber(ThrowCount));
	}
	else
	{
		IncreaseCount();
	}
	
}

void UThrowWidget::OnMinusButtonPressed()
{
	GetWorld()->GetTimerManager().SetTimer(TimeHanlde_Decrease,this,&UThrowWidget::DecreaseCount,0.2f,true,1.0f);
}

void UThrowWidget::OnPlusButtonPressed()
{
	GetWorld()->GetTimerManager().SetTimer(TimeHanlde_Increase, this, &UThrowWidget::IncreaseCount, 0.2f, true, 1.0f);
}

void UThrowWidget::OnMinusButtonReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeHanlde_Decrease);
}

void UThrowWidget::OnPlusButtonReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeHanlde_Increase);
}


void UThrowWidget::SetClickCountToZero()
{	
	ClickCount=0;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CountToZero);
}

void UThrowWidget::OnConfirmButtonClicked()
{
	//通过获取UI上输入的内容 得到FText类型 转FString 转 int
	ThrowCount = FCString::Atoi(*(CountEditableText->GetText().ToString()));
	if (InventoryRef->RemoveItemAtIndex(CurrentIndex,ThrowCount))
	{
		this->SetVisibility(ESlateVisibility::Hidden);
		InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(true);
		InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->CloseButton->SetIsEnabled(true);
	}
}

void UThrowWidget::OnCancelButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(true);
}
