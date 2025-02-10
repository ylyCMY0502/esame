// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Merchant.h"
#include "UserWidget/Inventory/ShopWidget.h"
#include "InventorySystem/Inventory.h"
#include "Kismet/GameplayStatics.h"
#include "Character/RBaseCharacter.h"
#include "UserWidget/Inventory/SellWidget.h"
#include "UserWidget/MainUserWidget.h"



void AMerchant::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter =Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	ShopWidget = CreateWidget<UShopWidget>(GetWorld(), LoadClass<UShopWidget>(GetWorld(), TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_Shop.WBP_Shop_C'")));
	
	ShopWidget->AddToViewport(2);
	ShopWidget->SetDesiredSizeInViewport(FVector2D(600, 720));
	ShopWidget->SetAnchorsInViewport(FAnchors(0, 0, 0, 0));
	ShopWidget->SetAlignmentInViewport(FVector2D(0,-0.3));
	
	ShopWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMerchant::OnIteractWith(ARBaseCharacter* Character)
{
	Super::OnIteractWith(Character);

	if (!bShopOpen)
	{
		ShopWidget->Merchant = this;
		ShopWidget->PlayerCharacter = PlayerCharacter;
		ShopWidget->SellWidget->InventoryRef = PlayerCharacter->InventoryRef;
		ShopWidget->SellWidget->ShopWidget = ShopWidget;

		ShopWidget->UpdatCoin();
		ShopWidget->GenerateItemList();

		ShopWidget->SetVisibility(ESlateVisibility::Visible);

		if ( ! Character->MainUserWidget->ShopWidget)
		{
			Character->MainUserWidget->ShopWidget=ShopWidget;
		}
		
		bShopOpen=true;
	} 
	else
	{
		ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		bShopOpen = false;
	}
	
}

void AMerchant::OnLeavePlayerRadius(ARBaseCharacter* Character)
{
	Super::OnLeavePlayerRadius(Character);
	ShopWidget->SetVisibility(ESlateVisibility::Hidden);
	bShopOpen = false;
}
