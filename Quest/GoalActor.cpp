// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/GoalActor.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSpriteComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"

// Sets default values
AGoalActor::AGoalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootCom=CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootCom"));
	RootComponent = DefaultRootCom;


	//目标点
	MinMapIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinMapIcon"));
	MinMapIcon->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UPaperSprite>Sprite(TEXT("PaperSprite'/Game/Textures/QuestTextures/NpcIcon_Sprite.NpcIcon_Sprite'"));
	MinMapIcon->SetSprite(Sprite.Object);

	MinMapIcon->SetRelativeLocation(FVector(0, 0, 150));
	MinMapIcon->SetRelativeRotation(FRotator(0, 0, 90));
	MinMapIcon->SetRelativeScale3D(FVector(2, 2, 2));
	MinMapIcon->SetGenerateOverlapEvents(false);
	MinMapIcon->SetCollisionProfileName(TEXT("NoCollision"));
	


	//目标区域
	MinMapAreaIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinMapAreaIcon"));
	MinMapAreaIcon->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UPaperSprite>AreaSprite(TEXT("/Script/Paper2D.PaperSprite'/Game/Textures/QuestTextures/GoalCircle_Sprite.GoalCircle_Sprite'"));
	MinMapAreaIcon->SetSprite(AreaSprite.Object);

	MinMapAreaIcon->SetRelativeLocation(FVector(0, 0, 150));
	MinMapAreaIcon->SetRelativeRotation(FRotator(0, 0, 90));
	MinMapAreaIcon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MinMapAreaIcon->SetGenerateOverlapEvents(false);
	MinMapAreaIcon->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);


}

// Called when the game starts or when spawned
void AGoalActor::BeginPlay()
{
	Super::BeginPlay();
	//设置主
	SetOwner(Cast<AActor>(UGameplayStatics::GetPlayerCharacter(this, 0)));
	//设置主不可见
	MinMapIcon->SetOwnerNoSee(true);
	MinMapAreaIcon->SetOwnerNoSee(true);
}

// Called every frame
void AGoalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoalActor::InitializeMinimapIcon(bool bUseRadius, float Radius, FLinearColor CricleColor)
{
	MinMapIcon->SetVisibility(!bUseRadius);
	MinMapAreaIcon->SetVisibility(bUseRadius);
	if (bUseRadius)
	{
		MinMapAreaIcon->SetRelativeScale3D(FVector(Radius,Radius,Radius));
		MinMapAreaIcon->SetSpriteColor(CricleColor);
	}
}


