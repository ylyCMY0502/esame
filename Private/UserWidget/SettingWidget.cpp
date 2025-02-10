// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SettingWidget.h"
#include "UMG/Public/Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Character/RBaseCharacter.h"
#include "RPGGameState.h"
#include "InventorySystem/Inventory.h"
#include "InventorySystem/Storage.h"
#include "UserWidget/MainUserWidget.h"

void USettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SaveButton->OnClicked.AddDynamic(this,&USettingWidget::OnSaveButtonClicked);
	LoadButton->OnClicked.AddDynamic(this, &USettingWidget::OnLoadButtonClicked);
	CloseButton->OnClicked.AddDynamic(this, &USettingWidget::OnCloseButtonClicked);
}

void USettingWidget::OnSaveButtonClicked()
{
	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	}
	
	if (PlayerCharacter)
	{
		if(PlayerCharacter->RPGGameState)
		{
			PlayerCharacter->RPGGameState->SaveGame();
		}
	
	}
	
	
}

void USettingWidget::OnLoadButtonClicked()
{
	
	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	}


	if (PlayerCharacter)
	{
		if (PlayerCharacter->RPGGameState)
		{
			PlayerCharacter->RPGGameState->LoadGame();
		}


	}
}


void USettingWidget::OnCloseButtonClicked()
{	
	/*this->
	SetVisibility(ESlateVisibility::Hidden);*/
	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	}
	PlayerCharacter->MainUserWidget->OnSettingButtonClicked();

}
