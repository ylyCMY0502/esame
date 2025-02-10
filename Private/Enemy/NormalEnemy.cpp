// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NormalEnemy.h"
#include "Enemy/NormalEnmeyController.h"
#include "AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Character/RBaseCharacter.h"
#include "AIModule/Classes/Perception/AISense_Sight.h"
#include "Components/CapsuleComponent.h"
#include "../RPGProject.h"
#include "Components/WidgetComponent.h"
#include "UserWidget/EnemyInfoWidget.h"
#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Other/StaticLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Skill/BaseElement.h"
#include "Character/Skill/BaseSkill.h"
#include "Character/Skill/SkillEnum.h"
#include "UserWidget/MainUserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/ArrowComponent.h"
#include "Quest/QuestManager.h"


#define  LOCTEXT_NAMESPACE "EnemyNameSpace"



// Sets default values
ANormalEnemy::ANormalEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AIController的类是
	AIControllerClass = ANormalEnmeyController::StaticClass();
	AIPerceptionCom = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionCom"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (AIPerceptionCom)
	{
		SightConfig->SightRadius = 800.0f;
		SightConfig->LoseSightRadius = 1000.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;

		//不设置true 
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	}

	AIPerceptionCom->ConfigureSense(*SightConfig);
	AIPerceptionCom->SetDominantSense(UAISense_Sight::StaticClass());
	//感知更新
	AIPerceptionCom->OnPerceptionUpdated.AddDynamic(this, &ANormalEnemy::OnSightPerceptionUpdated);


	//自己写的测试
	if (AIPerceptionCom->OnPerceptionUpdated.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT(" OnPerceptionUpdated IsBound yse "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnPerceptionUpdated IsBound no"));
	}
	

	//敌人血条场景UI
	EnemyWidgetCom= CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyWidgetCom"));
	EnemyWidgetCom->SetupAttachment(RootComponent);
	
	EnemyWidgetCom->SetRelativeLocation(FVector(0, 0, 80.0f));
	EnemyWidgetCom->SetDrawSize(FVector2D(160,30));
	static ConstructorHelpers::FClassFinder<UUserWidget>EnemyWidgetInfo(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Enemy/WBP_EnemyInfo.WBP_EnemyInfo_c'"));
	EnemyWidgetCom->SetWidgetClass(EnemyWidgetInfo.Class);
	EnemyWidgetCom->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	EnemyWidgetCom->SetTwoSided(true);
	EnemyWidgetCom->SetRelativeLocation(FVector(0,0,130));
	EnemyWidgetCom->SetVisibility(false);
	EnemyWidgetCom->SetWidgetSpace(EWidgetSpace::Screen);
	
	//敌人场景UI显示
	ShowUICollision = CreateDefaultSubobject<USphereComponent>(TEXT("ShowUICollision"));
	ShowUICollision->SetupAttachment(RootComponent);
	ShowUICollision->SetSphereRadius(800.0f);
	ShowUICollision->OnComponentBeginOverlap.AddDynamic(this,&ANormalEnemy::OnBeginOverlap);
	ShowUICollision->OnComponentEndOverlap.AddDynamic(this, &ANormalEnemy::OnEndOverlap);
	

	//弹道目标
	HitArrowCom = CreateDefaultSubobject<UArrowComponent>(TEXT("HitArrowCom"));
	HitArrowCom->SetupAttachment(RootComponent);

	//初始化血条
	CurrentHealth = TotalHealth;
	
}

// Called when the game starts or when spawned
void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();

	//获取UEnemyInfoWidget
	EnemyInfoWidget = Cast<UEnemyInfoWidget>(EnemyWidgetCom->GetUserWidgetObject());

	//给Mycontroller赋值为AIControllerClass（通过GetController来获取）
	MyController = Cast<ANormalEnmeyController>(GetController());
	MyController->Patrol();

	//初始位置
	StartLocation = GetActorLocation();

	EnemyWidgetCom->SetVisibility(false);
	
	InitWidgetText();
}

void ANormalEnemy::OnSightPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (bAggressive && !MyController->bWasAggroed)
	{
		for (auto Actor : UpdatedActors)
		{
			if (Cast<ARBaseCharacter>(Actor))
			{
				MyController->OnAggroedPulled(Cast<ARBaseCharacter>(Actor));
			}
		}
	}
}


void ANormalEnemy::NotifyHit()
{
	AttackRay();
}

void ANormalEnemy::AttackRay()
{
	int Range = 200.0f;
	FVector NorStartLocation = GetActorLocation();
	//UCapsuleComponent
	FVector EndLocatoion = NorStartLocation + GetCapsuleComponent()->GetForwardVector() * Range;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, NorStartLocation, EndLocatoion, EnemyHitChannel, QueryParams))
	{
		//UE_LOG(LogTemp, Warning, TEXT("jjjjjjjjjjjj"));

		ARBaseCharacter* Character = Cast<ARBaseCharacter>(HitResult.GetActor());
		if (Character)
		{
			IDamageableInterface* DamageableInterface = Cast<IDamageableInterface>(Character);

			if (DamageableInterface)
			{
				DamageableInterface->OnReceiveDamage(BaseDamage, DamageType, Element, CritChance, this, nullptr);
			}
		}
	}
}

void ANormalEnemy::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetBDead())
	{
		bInShowRange = true;
		//优化设置 EnemyWidgetCom->IsVisible()已经是true 了不设置了
		if (Cast<ARBaseCharacter>(OtherActor) && !EnemyWidgetCom->IsVisible())
		{
			EnemyWidgetCom->SetVisibility(true);
		}
	}
}

void ANormalEnemy::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!GetBDead())
	{
		bInShowRange = false;
		if (Cast<ARBaseCharacter>(OtherActor) && EnemyWidgetCom->IsVisible())
		{
			EnemyWidgetCom->SetVisibility(false);
		}
	}
}

void ANormalEnemy::InitWidgetText()
{
	if (EnemyInfoWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyInfoWidget is null"));
		return;
	}
	EnemyInfoWidget->LevelName->SetText(FText::Format(LOCTEXT("EnemyNameSpace","[Lv{0}.{1}]"),FText::AsNumber(Level),Name));
	FLinearColor CurrentColor;
	if (bAggressive)
	{
		CurrentColor = FLinearColor::Red;
	} 
	else
	{
		CurrentColor = FLinearColor::Green;
	}

	EnemyInfoWidget->LevelName->SetColorAndOpacity(CurrentColor);
}

void ANormalEnemy::OnDeath(AActor* Killer)
{
	MyController->AnimInstance->Montage_Stop(0.0f);
	bDead = true;
	//UCharacterMovementComponent;
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	ARBaseCharacter*TempCharacter =Cast<ARBaseCharacter>(Killer);
	if (TempCharacter)
	{
		TempCharacter->IncreaseChangeCurrentExp(ExpForKill);
		if (TempCharacter->SelectedEnemy == this)
		{
			OnSelectionEnd(SelectingCharacter);
		}

		TempCharacter->QuestManager->OnEnemyKilled(this->GetClass());

	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	////如果当前的敌人是在选择中的 死亡后隐藏
	//if (SelectingCharacter)
	//{
	//	SelectingCharacter->MainUserWidget->EnemyCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
	//	OnSelectionEnd(SelectingCharacter);
	//}


	GetWorld()->GetTimerManager().SetTimer(Timehandle_DelayDeath, this, &ANormalEnemy::OnDelayDeath, DelayDeath, false);
	EnemyWidgetCom->SetVisibility(false);
}


void ANormalEnemy::OnDelayDeath()
{
	GetWorld()->GetTimerManager().ClearTimer(Timehandle_DelayDeath);

	//场景中隐藏掉
	SetActorHiddenInGame(true);
	//重生
	if (bDoseRespawn)
	{
		GetWorld()->GetTimerManager().SetTimer(Timehandle_Respawn, this, &ANormalEnemy::Respawn, RespawnTime, false);
	}
	else
	{
		Destroy();
	}
}

void ANormalEnemy::Respawn()
{
	UE_LOG(LogTemp, Warning, TEXT("Respawn............................"))

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	bDead = false;
	CurrentHealth = TotalHealth;
	UpdateHealthBar();
	InitWidgetText();
	SetActorLocation(StartLocation);

	MyController->bIsPatrolling = true;
	MyController->bWasAggroed = false;
	MyController->TargetActor = nullptr;
	MyController->CurrentAttackIndex = 0;
	MyController->bIsRunningBack = false;

	GetCharacterMovement()->MaxWalkSpeed = MyController->PatroyWalkSpeed;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	EnemyWidgetCom->SetVisibility(bInShowRange);
	SetActorHiddenInGame(false);
	
	MyController->Patrol();
}


void ANormalEnemy::OnSelectable(class ARBaseCharacter* Character)
{
	if (!bDead && !bSelected)
	{
		UE_LOG(LogTemp, Warning, TEXT("--------OnSelectable-----------"));
		SelectingCharacter = Character;
		bSelected = true;
		GetMesh()->SetRenderCustomDepth(true);
		Character->MainUserWidget->EnemyNaneLevel->SetText(FText::Format(LOCTEXT("EnemyNameSpace", "Lv{0}.{1}"), FText::AsNumber(Level), Name));
		UpdateHealthBar();
		//UCanvasPanel
		Character->MainUserWidget->EnemyCanvasPanel->SetVisibility(ESlateVisibility::Visible);
		Character->SelectedEnemy=this;
	}
}


void ANormalEnemy::OnSelectionEnd(class ARBaseCharacter* Character)
{
	if (bSelected)
	{
		bSelected = false;
		//关闭高亮显示
		GetMesh()->SetRenderCustomDepth(false);
		SelectingCharacter = nullptr;
		Character->MainUserWidget->EnemyCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
		Character->SelectedEnemy = nullptr;
	}
}



void ANormalEnemy::UpdateHealthBar()
{
	//UProgressBar
	if (EnemyInfoWidget)
	{
		EnemyInfoWidget->HealthBar->SetPercent(CurrentHealth / TotalHealth);
	}

	//UI上面显示
	if (bSelected && SelectingCharacter)
	{
		SelectingCharacter->MainUserWidget->EnemyHPBar->SetPercent(CurrentHealth / TotalHealth);
		SelectingCharacter->MainUserWidget->EnemyHPText->SetText(FText::Format(LOCTEXT("EnemyNameSpace", "{0}/{1}]"), FText::AsNumber(FMath::RoundHalfToZero(CurrentHealth)), FText::AsNumber(TotalHealth)));
	}

}

// Called every frame
void ANormalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ANormalEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



void ANormalEnemy::OnReceiveDamage(float BaseDamage, EDamageTypeSelected Type, TSubclassOf<ABaseElement>Element, int CritChance, AActor* Attacker, ABaseSkill* Skill)
{
	if (!GetBDead() && !UStaticLibrary::BIsEnemy(Attacker))
	{
		CurrentHealth = FMath::Clamp(CurrentHealth-(UStaticLibrary::CaculateFinalDamage(BaseDamage, CritChance, Element, this->Element)),0, TotalHealth);
		
		
		UpdateHealthBar();
		if (CurrentHealth < 0.5)
		{
			OnDeath(Attacker);
		}
		else
		{
			//可以不用
			MyController->OnAggroedPulled(Attacker);
		}
	}
}

#undef LOCTEXT_NAMESPACE