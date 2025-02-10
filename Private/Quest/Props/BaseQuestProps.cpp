// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/Props/BaseQuestProps.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "../../../Public/UserWidget/Quest/InteractionWidget.h"
#include "../../../Public/Character/RBaseCharacter.h"
#include "../../../Public/Quest/QuestManager.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ABaseQuestProps::ABaseQuestProps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent=SceneComp;
	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);

	InteractionWidget=CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(RootComponent);

	InteractionWidget->SetCollisionProfileName(TEXT("NoCollision"));
	InteractionWidget->SetGenerateOverlapEvents(false);

	ConstructorHelpers::FClassFinder<UInteractionWidget>US(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Quest/WBP_Interaction.WBP_Interaction_C'"));
	InteractionWidget->SetWidgetClass(US.Class);

	ParticleSystemCom = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemCom"));
	ParticleSystemCom->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ABaseQuestProps::BeginPlay()
{
	Super::BeginPlay();
	UInteractionWidget*PropsUI=Cast<UInteractionWidget>(InteractionWidget->GetUserWidgetObject());
	if (PropsUI)
	{ 
		UE_LOG(LogTemp, Warning, TEXT("PropsUI "));
		PropsUI->SetNameText(Name);
		PropsUI->SetInteractionText(InteractionTFext);
	}
}

// Called every frame
void ABaseQuestProps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABaseQuestProps::OnEnterPlayerRadius(ARBaseCharacter* Character)
{
	InteractionWidget->SetVisibility(true);
}

void ABaseQuestProps::OnLeavePlayerRadius(ARBaseCharacter* Character)
{
	InteractionWidget->SetVisibility(false);
}

void ABaseQuestProps::OnIteractWith(ARBaseCharacter* Character)
{
	Character->ObtaninedPorps.Add(this->GetClass());
	Character->QuestManager->OnObjectFound(this->GetClass());
	//Çå³ý×Ô¼º
	Destroy();
}