// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/BsaeNPC.h"
#include "Components/WidgetComponent.h"
#include "Character/RBaseCharacter.h"
#include "Quest/QuestManager.h"
#include "UserWidget/Quest/InteractionWidget.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSpriteComponent.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget/Quest/MessageWidget.h"
#include "GameFramework/Actor.h"


// Sets default values
ABsaeNPC::ABsaeNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InterctionWidget =CreateDefaultSubobject<UWidgetComponent>(TEXT("InterctionWidget"));
	InterctionWidget->SetupAttachment(RootComponent);
	//����UI ����
	InterctionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InterctionWidget->SetCollisionProfileName(TEXT("NoCollision"));
	InterctionWidget->SetGenerateOverlapEvents(false);
	ConstructorHelpers::FClassFinder<UUserWidget>InterctionWidgetUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Quest/WBP_Interaction.WBP_Interaction_C'"));
	InterctionWidget->SetWidgetClass(InterctionWidgetUI.Class);

	//���˳���UI��ʾ
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.f));

	//�Ի�UI
	MessageWidgetCom = CreateDefaultSubobject<UWidgetComponent>(TEXT("MessageWidgetCom"));
	MessageWidgetCom->SetupAttachment(RootComponent);
	//����UI ����
	MessageWidgetCom->SetWidgetSpace(EWidgetSpace::Screen);
	MessageWidgetCom->SetCollisionProfileName(TEXT("NoCollision"));
	MessageWidgetCom->SetGenerateOverlapEvents(false);
	ConstructorHelpers::FClassFinder<UUserWidget>MessageWidgetUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Quest/WBP_Message.WBP_Message_C'"));
	MessageWidgetCom->SetWidgetClass(MessageWidgetUI.Class);

	QuestIcon=CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("QuestIcon"));
	QuestIcon->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UPaperSprite>QuestSprite(TEXT("/Script/Paper2D.PaperSprite'/Game/Textures/QuestTextures/Quest_Icon_Sprite.Quest_Icon_Sprite'"));
	QuestIcon->SetSprite(QuestSprite.Object);
	QuestIcon->SetSpriteColor(FLinearColor::Yellow);

	QuestIcon->SetRelativeLocation(FVector(0, 0, 150));
	QuestIcon->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
	QuestIcon->SetGenerateOverlapEvents(false);
	QuestIcon->SetCollisionProfileName(TEXT("NoCollision"));

}

// Called when the game starts or when spawned
void ABsaeNPC::BeginPlay()
{
	Super::BeginPlay();


	InterctionWidget->SetVisibility(false);
	MessageWidgetCom->SetVisibility(false);

	

	//����ͼ�ϵ�����������
	if (Cast<UInteractionWidget>(InterctionWidget->GetUserWidgetObject()))
	{
		UE_LOG(LogTemp, Warning, TEXT("InterctionWidget null"));
		Cast<UInteractionWidget>(InterctionWidget->GetUserWidgetObject())->SetNameText(Name);
	}
	
	QuestIcon->SetVisibility(bHasQuest);
	QuestIcon->SetWorldRotation(FRotator(0, 90, -90));

	SetOwner(Cast<AActor>(UGameplayStatics::GetPlayerCharacter(this,0)));
	QuestIcon->SetOwnerNoSee(true);


	MessageWidget=Cast<UMessageWidget>(MessageWidgetCom->GetUserWidgetObject());


}

void ABsaeNPC::ResetMessage()
{
	MessageWidgetCom->SetVisibility(false);
	InterctionWidget->SetVisibility(bInPlayRadius);
	bCanTalkTo=true;
	GetWorldTimerManager().ClearTimer(TimeHandle_ResetMessage);
}

// Called every frame
void ABsaeNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABsaeNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABsaeNPC::OnEnterPlayerRadius(ARBaseCharacter* Character)
{
	bInPlayRadius=true;
	InterctionWidget->SetVisibility(true);
}

void ABsaeNPC::OnLeavePlayerRadius(ARBaseCharacter* Character)
{
	bInPlayRadius=false;
	InterctionWidget->SetVisibility(false);
}

void ABsaeNPC::OnIteractWith(ARBaseCharacter* Character)
{
//�ж���û������ �����������ʾ����  û���������ʾ��̸������
	if(MyQuset)
	{ 
		if(!Character->QuestManager->AllQuestClasses.Contains(MyQuset))
		{
			Character->QuestManager->AddNewQuset(MyQuset);
			QuestIcon->SetVisibility(false);
		}
		else
		{
			OnTalkWith(Character);
		}
	}
	else
	{
		OnTalkWith(Character);
	}
}

void ABsaeNPC::OnTalkWith(class ARBaseCharacter* Character)
{
	ShowMessage( Message,  DefaultDuration, Character);
}

void ABsaeNPC::ShowMessage(FText Message, float DefaultDuration, ARBaseCharacter* Character)
{
	if (bCanTalkTo)
	{
		InterctionWidget->SetVisibility(false);
		MessageWidget->SetMessage(Message);
		MessageWidgetCom->SetVisibility(true);
		bCanTalkTo=false;
		Character->QuestManager->OnTalkToNPC(this->GetClass(),NPCId);
		GetWorldTimerManager().SetTimer(TimeHandle_ResetMessage,this,&ABsaeNPC::ResetMessage,DefaultDuration,false);
	}
}

