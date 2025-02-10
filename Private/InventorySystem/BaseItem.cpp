// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/BaseItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "../../Public/UserWidget/Quest/InteractionWidget.h"
#include "UMG/Public/Components/WidgetComponent.h"
#include "Character/RBaseCharacter.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/MainUserWidget.h"


// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	

	StaticMeshComp=CreateDefaultSubobject< UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetGenerateOverlapEvents(false);

	BoxCollisonComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisonComp"));
	BoxCollisonComp->SetupAttachment(RootComponent);

	InteractionWidgetComp=CreateDefaultSubobject<UWidgetComponent>(TEXT("UWidgetComponent"));
	InteractionWidgetComp->SetupAttachment(RootComponent);
	InteractionWidgetComp->SetRelativeScale3D(FVector(0.1,0.1,0.1));
	InteractionWidgetComp->SetCollisionProfileName(TEXT("NoCollision"));
	InteractionWidgetComp->SetGenerateOverlapEvents(false);
	InteractionWidgetComp->SetVisibility(false);
	InteractionWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	ConstructorHelpers::FClassFinder<UInteractionWidget>US(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Quest/WBP_Interaction.WBP_Interaction_C'"));
	InteractionWidgetComp->SetWidgetClass(US.Class);
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
	UInteractionWidget* PropsUI = Cast<UInteractionWidget>(InteractionWidgetComp->GetUserWidgetObject());
	if (PropsUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("PropsUI "));
		PropsUI->SetNameText(Name);
		PropsUI->SetInteractionText(InteractionTFext);
	}
}




// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ABaseItem::OnEnterPlayerRadius(ARBaseCharacter* Character)
{
	InteractionWidgetComp->SetVisibility(true);
}

void ABaseItem::OnLeavePlayerRadius(ARBaseCharacter* Character)
{
	InteractionWidgetComp->SetVisibility(false);
}

void ABaseItem::OnIteractWith(ARBaseCharacter* Character)
{

	int RestAmount= Character->InventoryRef->AddItem(this->GetClass(), Amount);
	if (Amount != RestAmount)
	{	
		//背包的空间不足（物品群满）此物品还剩下50个是时候  Amount是地上物品的数量
		Character->MainUserWidget->AddItemToObtainQueue(this->GetClass(), Amount-RestAmount) ;
	} 
	
	if (RestAmount > 0)
	{
		Amount = RestAmount;

		//保存的物品
		if (ID !=-1)
		{
			Character->InventoryRef->lootedPickups.Add(FSavedPickup{ID,RestAmount});
		}

	}
	else
	{
		if (ID != -1)
		{
			Character->InventoryRef->lootedPickups.Add(FSavedPickup{ ID,0 });
		}
		//清除自己
		Destroy();
	}

	
}

void ABaseItem::OnUsed()
{
	if (InventoryRef->RemoveItemAtIndex(Index,1))
	{
		UE_LOG(LogTemp,Warning,TEXT("You used the %s"),*ItemInfo.Name.ToString());
		Destroy();//?
	}
}
