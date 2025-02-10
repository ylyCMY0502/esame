// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemMap.h"
#include "../../Public/UserWidget/Inventory/ReadableItemWidget.h"
#include "Components/Border.h"


void AItemMap::OnUsed()
{
	
	//InventoryRef->PlayerCharacter->MainUserWidget->ReadableContainer->SetVisibility(ESlateVisibility::Visible);
	InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->SetIsEnabled(false);
	Map  =CreateWidget<UReadableItemWidget>(GetWorld(),LoadClass<UReadableItemWidget>(nullptr,TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_RadableItem.WBP_RadableItem_C'")));
	InventoryRef->PlayerCharacter->MainUserWidget->ReadableContainer->AddChild(Map);
	 Map->ItemMap=this;
	FInputModeUIOnly InputMode;
	InventoryRef->PlayerCharacter->RPlayerController->SetInputMode(InputMode);
}

void AItemMap::OnReadOver()
{
	InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->SetIsEnabled(true);

	if (Map)
	{	
		Map->RemoveFromParent();
	}

	FInputModeGameAndUI InputMode;
	InventoryRef->PlayerCharacter->RPlayerController->SetInputMode(InputMode);
}

