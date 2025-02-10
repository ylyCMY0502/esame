// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Character/MouseCursor.h"
#include "../RPGProject.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "UserWidget/MainUserWidget.h"
#include "Engine/DataTable.h"
#include "Character/Info/CharacterInfo.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Character/Skill/BaseSkill.h"
#include "UserWidget/SkillHotkey.h"
#include "Enemy/NormalEnemy.h"
#include "Other/StaticLibrary.h"
#include "Interface/SelectableInterface.h"
#include "Character/Skill/MissileSkill.h"
#include "Character/Skill/BuffSkill.h"
#include "UserWidget/BuffWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/SkillTreeComponent.h"
#include "Quest/QuestManager.h"
#include "Components/CapsuleComponent.h"
#include "Interface/InterationInterface.h"
#include "GameFramework/Actor.h"
#include "NPC/BsaeNPC.h"
#include "Quest/BaseQuest.h"
#include "UserWidget/Quest/QuestJournalWidget.h"
#include "UserWidget/Quest/QuestListEntryWidget.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UserWidget/Inventory/ActionMenuWidget.h"
#include "UserWidget/Inventory/ThrowWidget.h"
#include "InventorySystem/ItemStaff.h"
#include "Particles/ParticleSystemComponent.h"
#include "UserWidget/Inventory/CraftingMenuWidget.h"
#include "UserWidget/Inventory/ShopWidget.h"
#include "UserWidget/Inventory/SellWidget.h"
#include "RPGGameState.h"
#include "UserWidget/Inventory/StorageWidget.h"
//#include "GameFramework/Actor.h"

// Sets default values
ARBaseCharacter::ARBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//创建摄像机弹簧组件并绑定+
	RSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("RSpringArm"));
	RSpringArm->SetupAttachment(RootComponent);
	//RSpringArm->bDoCollisionTest = false;
	RSpringArm->TargetArmLength = 900;

	RCamera=CreateDefaultSubobject<UCameraComponent>(TEXT("RCamera"));
	RCamera->SetupAttachment(RSpringArm);
	
	//用于任务的胶囊组件
	InterationComponent=CreateDefaultSubobject<UCapsuleComponent>(TEXT("InterationComponent"));
	InterationComponent->SetupAttachment(RootComponent);

	InterationComponent->SetRelativeLocation(FVector(120.f, 0, 0));
	InterationComponent->SetRelativeRotation(FRotator(90.f,0,0));
	InterationComponent->SetCapsuleHalfHeight(220);
	InterationComponent->SetCapsuleRadius(90);


	//超重
	OverloadParticleComp=CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OverloadParticleComp"));
	OverloadParticleComp->SetupAttachment(GetMesh());
	OverloadParticleComp->Deactivate();

	InterationComponent->OnComponentBeginOverlap.AddDynamic(this, &ARBaseCharacter::OnInteractionComPBeginOverlap);
	InterationComponent->OnComponentEndOverlap.AddDynamic(this, &ARBaseCharacter::OnInteractionComEndOverlap);

	//加载贴花的预设
	ConstructorHelpers::FClassFinder<AMouseCursor>MouseCursorDecal(TEXT("'/Game/Blueprints/Character/BP_MouseCursor'"));
	SpawnMouseCursor = MouseCursorDecal.Class;


	//人物旋转默认
	bUseControllerRotationYaw = false;
	RCamera->bUsePawnControlRotation = true;
	RSpringArm->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;



	//经验
	CurrentLevel = 1;
	NeededExpToNextLeve = FMath::FloorToInt((FMath::Pow(CurrentLevel - 1, 3) + 60) / 5 * ((CurrentLevel - 1) * 2 + 60) + 60);

	//头像
	PortraitComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortraitComponent"));
	PortraitComponent->SetupAttachment(GetMesh(),"neck_01");

	//PortraitComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "neck_02");


	PortraitComponent->SetRelativeRotation(FRotator(0.f, -90.f, 90.f));
	PortraitComponent->SetRelativeLocation(FVector(25.f,35.f,0.f));


	//按键赋值
	Keys.Add(EKeys::One);
	Keys.Add(EKeys::Two);
	Keys.Add(EKeys::Three);
	Keys.Add(EKeys::Four);
	Keys.Add(EKeys::Five);
	Keys.Add(EKeys::Six);
	Keys.Add(EKeys::Seven);
	Keys.Add(EKeys::Eight);
	Keys.Add(EKeys::Nine);
	Keys.Add(EKeys::Zero);
	Keys.Add(EKeys::F1);
	Keys.Add(EKeys::F2);
	Keys.Add(EKeys::F3);
	Keys.Add(EKeys::F4);
	Keys.Add(EKeys::F5);
	Keys.Add(EKeys::F6);
	Keys.Add(EKeys::F7);
	Keys.Add(EKeys::F8);
	Keys.Add(EKeys::F9);
	Keys.Add(EKeys::F10);
	/*Keys.Add(EKeys::F11);
	Keys.Add(EKeys::F12);*/


	//技能树
	SKillTreeCom = CreateDefaultSubobject<USkillTreeComponent>(TEXT("USkillTreeComponent"));
}



// Called when the game starts or when spawned
void ARBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	RPlayerController = UGameplayStatics::GetPlayerController(GWorld, 0);
	RPlayerController->bShowMouseCursor = true;//显示鼠标

	
	FActorSpawnParameters Parameter;
	Parameter.Owner=this;
	Parameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	//背包系统
	InventoryRef = GetWorld()->SpawnActor<AInventory>(InventoryClass, Parameter);

	RPGGameState = GetWorld()->SpawnActor<ARPGGameState>(Parameter);
	RPGGameState->PlayerCharacter = this;
	

	//添加主UI
	MainUserWidget =CreateWidget<UMainUserWidget>(GetWorld(),
	LoadClass<UMainUserWidget>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UI/WBP_Mian.WBP_Mian_C'")));
	//生成技能按键组
	MainUserWidget->GenerateHotkeys(Keys, KeysPerRow);

	DefaultSpeed = GetCharacterMovement()->MaxWalkSpeed;

	//---------------加载---------------------
	// 
	ReadDate();


	//if (!UGameplayStatics::DoesSaveGameExist(RPGGameState->SaveSlotName, 0))
	//{
	//	//读取表格中的数据
	//	ReadDate();

	//}
	//else
	//{
	//	
	//	RPGGameState->LoadGame();
	//	MainUserWidget->SetHPProgressBar(CurrentHp / TotalHp);
	//	MainUserWidget->SetMPProgressBar(CurrentMp / TotalMp);
	//}


	

	MainUserWidget->AddToViewport();

	MainUserWidget->InventoryWidget->InventoryRef=InventoryRef;
	MainUserWidget->InventoryWidget->GenerateSlotWidget();
	//MainUserWidget->InventoryWidget->UpdateCoinText(CurrentCoin);

	MainUserWidget->Inventory=InventoryRef;
	

	MainUserWidget->QuestManager = QuestManager;
	//背包中的丢弃
	MainUserWidget->ThrowWidget->InventoryRef = InventoryRef;
	//创库
	MainUserWidget->StorageWidget->PlayerInventoryRef= InventoryRef;

	


	
	//经验
	//IncreaseChangeCurrentExp(0);
	BeginPlayChangeCurrentExp();
	MainUserWidget->SetLevelText(FText::AsNumber(CurrentLevel));


	//技能生成
	//GenerateStartingSkills();

	SKillTreeCom->SetupTree();

	//任务系统界面生成
	QuestManager = GetWorld()->SpawnActor<AQuestManager>(QuestManagerClass, Parameter);
	QuestManager->PlayerCharacter = this;
	QuestManager->MainUserWidget = MainUserWidget;
	//MainUserWidget->QuestManager = QuestManager;

	MainUserWidget->QuestJournal->Initialize(QuestManager);

	MainUserWidget->InventoryWidget->ActionMenu->InventoryRef=InventoryRef;

	InventoryRef->UpdateWeight();
	MainUserWidget->CraftingMenu->IntiCraftaingMenu(InventoryRef);
	


	MainUserWidget->GenerateItemHotkey(ItemKeys);

}



//设置移动方向
void ARBaseCharacter::MoveForward(float Value)
{
	//AddMovementInput(GetActorForwardVector(), Value);
	
	
	if (Value != 0)
	{
		CancleMissle();
		//如果是鼠标移动取消移动 清楚光标
		if (bHasMouseMoveCommand)
		{
			CancleMoveToCursor();
		}


		//获取
		FRotator Rotation = GetControlRotation();
		FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector Dir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Dir, Value);
		
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(Vec.Roll));
		//任务移动更新UI步数
		QuestManager->OnPlayerMove();
	}
}

void ARBaseCharacter::MoveRight(float Value)
{
	if (Value != 0)
	{
		CancleMissle();

		//如果是鼠标移动取消移动 清除光标
		if (bHasMouseMoveCommand)
		{
			CancleMoveToCursor();
		}

		FRotator Rotation = GetControlRotation();
		FRotator YawRotation(0, Rotation.Yaw, 0);
		FVector Dir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Dir, Value);
		//任务移动更新UI步数
		QuestManager->OnPlayerMove();
	}
}





//通过AI移动光标处
void ARBaseCharacter::SetNewMoveDestination(const FVector Pos)
{
	float Dis = FVector::Dist(GetActorLocation(), Pos);
	if (Dis > 100.f)
	{
		//通过AI蓝图来移动到贴花上
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(RPlayerController, Pos);
	}
	//任务移动更新UI步数
	QuestManager->OnPlayerMove();
}


//移动到光标
void ARBaseCharacter::MoveToCursor()
{
	//碰撞结果
	FHitResult Hit;
	//鼠标与Actor碰撞
	RPlayerController->GetHitResultUnderCursor(CursorTraceChannel, false, Hit);
	
	if (Hit.bBlockingHit)//是否有碰撞
	{
		//转化成选择的接口
		ISelectableInterface* SelectableInterface = Cast<ISelectableInterface>(Hit.GetActor());
		
		if (SelectableInterface)
		{
			//两次选择的是不是同一个,如果是
			if (SelectedActor== Hit.GetActor())
			{
				SelectedActor = Hit.GetActor();
				//将选择的转为接口 调用接口的函数
				ISelectableInterface * SelectableActorInterface = Cast<ISelectableInterface>(SelectedActor);
				if (SelectableActorInterface)
				{
					SelectableActorInterface->OnSelectable(this);
				}
			} 
			//两点击的不是同一个
			else
			{
				//取消之前的的
				ISelectableInterface* SelectableActorInterface = Cast<ISelectableInterface>(SelectedActor);
				if (SelectableActorInterface)
				{
					SelectableActorInterface->OnSelectionEnd(this);
				}
				//当期点击的设置为高亮
				if (Cast<ISelectableInterface>(Hit.GetActor()))
				{
					Cast<ISelectableInterface>(Hit.GetActor())->OnSelectable(this);
					SelectedActor = Hit.GetActor();
				}
			}
		} 
		else
		{
			//将之前点击的设置为空
			if (SelectedActor)
			{
				Cast<ISelectableInterface>(SelectedActor)->OnSelectionEnd(this);
				SelectedActor = nullptr;
			}

			FActorSpawnParameters Parameter;
			Parameter.Owner = this;
			//UWorld  生成贴花
			CurrentMouseCursor = GetWorld()->SpawnActor<AMouseCursor>(SpawnMouseCursor, Hit.Location, FRotator::ZeroRotator, Parameter);
			SetNewMoveDestination(Hit.Location);
		}

		
	}
}

//取消光标
void ARBaseCharacter::CancleMoveToCursor()
{
	if (CurrentMouseCursor != nullptr )
	{
		bHasMouseMoveCommand = false;
		//停止移动
		GetCharacterMovement()->StopMovementImmediately();
		
		CurrentMouseCursor->Destroy();
		CurrentMouseCursor = nullptr; //e
	}
}

//设置光标点击
void ARBaseCharacter::OnSetDestinationPressed()
{
	CancleMoveToCursor();//取消之前的光标显示
	bHasMouseMoveCommand = true;
	MoveToCursor();//E

}




//镜头远近
void ARBaseCharacter::CameraZoomIn()
{
	RSpringArm->TargetArmLength = FMath::Clamp(RSpringArm->TargetArmLength - CameraZoomSpeed, MidCameraZoom, MaxCameraZoom);
}

void ARBaseCharacter::CameraZoomOut()
{
	RSpringArm->TargetArmLength = FMath::Clamp(RSpringArm->TargetArmLength + CameraZoomSpeed, MidCameraZoom, MaxCameraZoom);
}




void ARBaseCharacter::ToogleShowSkillTree()
{
	UE_LOG(LogTemp, Warning, TEXT("ToogleShowSkillTree........"));
	SKillTreeCom->HanldShowCommand();
	
}

void ARBaseCharacter::ToogleShowQuest()
{
	if (MainUserWidget->bQuestVisibling)
	{
		MainUserWidget->QuestJournal->SetVisibility(ESlateVisibility::Hidden);
		MainUserWidget->bQuestVisibling=false;
	}
	else
	{
		MainUserWidget->QuestJournal->SetVisibility(ESlateVisibility::Visible);
		MainUserWidget->bQuestVisibling = true;
	}
	
}

void ARBaseCharacter::ToogleShowInventroy()
{
	if (MainUserWidget->bInventoryVisibling)
	{
		MainUserWidget->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		MainUserWidget->bInventoryVisibling=false;
	} 
	else
	{
		MainUserWidget->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		MainUserWidget->bInventoryVisibling = true;
	}
}

void ARBaseCharacter::ToogleShowCrafting()
{
	if (MainUserWidget->bCraftingVisibling)
	{
		MainUserWidget->CraftingMenu->SetVisibility(ESlateVisibility::Hidden);
		MainUserWidget->bCraftingVisibling = false;
	}
	else
	{
		MainUserWidget->CraftingMenu->SetVisibility(ESlateVisibility::Visible);
		MainUserWidget->bCraftingVisibling = true;
	}
}



void ARBaseCharacter::ReadDate()
{
	//加载表格
	UDataTable* CharacterInfo=Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, TEXT("DataTable'/Game/Blueprints/Character/Info/CharacterInfo.CharacterInfo'")));
	if (CharacterInfo == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterInfo is null"));
	}
	//TEXT 1是行    2是报错
	FCharacterInfo* CharacterInfoRead =CharacterInfo->FindRow<FCharacterInfo>(TEXT("S1"), TEXT("NULL"));
	
	if(CharacterInfoRead)
	{ 
		CharacterName = CharacterInfoRead->CurrentName;
		TotalHp = CharacterInfoRead->StartHP;
		TotalMp = CharacterInfoRead->StartMP;

		CurrentHp = TotalHp;
		CurrentMp=TotalMp;


		ChangeCurrentHP(0);
		ChangeCurrentMP(0);
		//IncreaseChangeCurrentExp(CurrentLevel);
	}
}

float ARBaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	ChangeCurrentHP(Damage * -1);
	return Damage;
}




void ARBaseCharacter::OnReceiveDamage(float BaseDamage, EDamageTypeSelected Type, TSubclassOf<ABaseElement>Element, int CritChance, AActor* Attacker, ABaseSkill* Skill)
{
	if (UStaticLibrary::BIsEnemy(Attacker) && BaseDamage !=0)
	{
		ChangeCurrentHP(-1 * ( UStaticLibrary::CaculateFinalDamage(BaseDamage, CritChance, Element,this->Element) ));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(UStaticLibrary::CaculateFinalDamage(BaseDamage, CritChance, Element, this->Element)));

	}
}

// Called every frame
void ARBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//绑定移动按键
	PlayerInputComponent->BindAxis("MoveForward", this, &ARBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);
	//跳跃
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//鼠标点击
	PlayerInputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ARBaseCharacter::OnSetDestinationPressed);

	//镜头远近
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ARBaseCharacter::CameraZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed,this, &ARBaseCharacter::CameraZoomOut);

	//按键绑定
	PlayerInputComponent->BindAction("AnyKey", IE_Pressed, this, &ARBaseCharacter::OnAnyKeyPressed);
	
	//绑定技能按键
	PlayerInputComponent->BindAction("ToogleShowSkillTree", IE_Pressed, this, &ARBaseCharacter::ToogleShowSkillTree);

	//E任务交互
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARBaseCharacter::InteractionToNPC);

	//Tab切换任务ui动画
	PlayerInputComponent->BindAction("PlaySideOutAnim", IE_Pressed, this, &ARBaseCharacter::PlaySideOutAnimation);
	

	//任务快捷键
	PlayerInputComponent->BindAction("ToogleShowQuest", IE_Pressed, this, &ARBaseCharacter::ToogleShowQuest);
	//Test任务的完成
	PlayerInputComponent->BindAction("TestCompleteQuset", IE_Pressed, this, &ARBaseCharacter::TestCompleteQuset);
	PlayerInputComponent->BindAction("TestFaileQusest", IE_Pressed, this, &ARBaseCharacter::TestFaileQusest);
	//Shift
	PlayerInputComponent->BindAction("Shift", IE_Pressed, this, &ARBaseCharacter::OnShiftPressed);
	PlayerInputComponent->BindAction("Shift", IE_Released, this, &ARBaseCharacter::OnShiftReleased);


	//背包快捷键
	PlayerInputComponent->BindAction("ToogleShowInventroy", IE_Pressed, this, &ARBaseCharacter::ToogleShowInventroy);

	//合成
	PlayerInputComponent->BindAction("ToogleShowCrafting", IE_Released, this, &ARBaseCharacter::ToogleShowCrafting);

	

	
}



////生成技能
//void ARBaseCharacter::GenerateStartingSkills()
//{
//	
//
//	for (auto Skill : StartingSkills)
//	{
//		FActorSpawnParameters Params;
//		Params.Owner=this;
//		//生成碰撞处理覆盖
//		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//		ABaseSkill* TempSkill =GetWorld()->SpawnActor<ABaseSkill>(Skill, Params);
//		//TempSkill->SetPlayerReference(this);
//		
//		
//		//USkillHotkey  ABaseSkill
//		for (auto HotKey : MainUserWidget->GetAllHotkeySlots())
//		{
//			if (HotKey && !HotKey->GetAssigendSpell())
//			{
//				
//
//				HotKey->AssigenSpell(TempSkill);
//				
//				break;
//			}
//
//		}
//
//	}
//}


//AnyKey 按键
void ARBaseCharacter::OnAnyKeyPressed(FKey Key)
{
	UE_LOG(LogTemp, Warning, TEXT("--HandleItemHotkeyPress--- "));
	if (bCanFindKey)
	{
		bCanFindKey = false;
		UE_LOG(LogTemp, Warning, TEXT("--HandleItemHotkeyPress--bCanFindKey- "));
		InventoryRef->HandleItemHotkeyPress(Key);

		for (auto HotKeySlot:MainUserWidget->GetAllHotkeySlots())
		{
			/*if (RPlayerController->IsInputKeyDown(HotKeySlot->GetKey()))
			{
				if (HotKeySlot->GetAssigendSpell())
				{
					HotKeySlot->GetAssigendSpell()->OnTryCastSpell();
					break;
				}
			}*/

			if (Key== HotKeySlot->GetKey())
			{
				if (HotKeySlot->GetAssigendSpell())
				{
					HotKeySlot->GetAssigendSpell()->OnTryCastSpell();
					break;
				}
			}



		}

		
		bCanFindKey = true;
		
	}
}



void ARBaseCharacter::CancleMissle()
{
	if (MissileSkill)
	{
		//立即马上 停止移动
		GetCharacterMovement()->StopMovementImmediately();

		MissileSkill->SotpCaculate();
	}
	
}




void ARBaseCharacter::IncreaseLeveL()
{
	CurrentLevel++;
	NeededExpToNextLeve = FMath::FloorToInt((FMath::Pow(CurrentLevel - 1, 3) + 60) / 5 * ((CurrentLevel - 1) * 2 + 60) + 60);
	MainUserWidget->SetLevelText(FText::AsNumber(CurrentLevel));

	//更新任务UI颜色
	for (UQuestListEntryWidget* QuestListEntryWidget:MainUserWidget->QuestJournal->AllQuestListEntries)
	{
		QuestListEntryWidget->UpdateLevelColor();
	}

	if(MainUserWidget->QuestJournal->CurrentUQuestListEntryWidget)
	{
		MainUserWidget->QuestJournal->UpdateSuggestedLevelColor();
	}
}


void ARBaseCharacter::BeginSpellCast(ABaseSkill* Skill)
{
	bIsCasting = true;
	CurrentSkill = Skill;
	for (auto HotKeySlot : MainUserWidget->GetAllHotkeySlots())
	{
		//分配的技能有没有
		if (HotKeySlot->GetAssigendSpell())
		{
			
			//分配的技能是不是传入的当前技能
			if (CurrentSkill != HotKeySlot->GetAssigendSpell())
			{
				//不是当前的技能 就让按键失效
				HotKeySlot->DisableHotkey();
			}
		}
	}
}


void ARBaseCharacter::EndSpellCast(ABaseSkill* Skill)
{
	bIsCasting = false;
	CurrentSkill = Skill;
	
	for (auto HotKeySlot : MainUserWidget->GetAllHotkeySlots())
	{
		
		//分配的技能有没有
		if (HotKeySlot->GetAssigendSpell())
		{
			//分配的技能是不是传入的当前技能
			if (CurrentSkill != HotKeySlot->GetAssigendSpell())
			{
				//在是放中不是当前的技能 就让按键开启
				HotKeySlot->EnableHotkey();

			}
		}
	}
}




UBuffWidget* ARBaseCharacter::AddBuff(ABuffSkill* SkillBuff)
{
	if (Buffs.Contains(SkillBuff))
	{
		return nullptr;
	}
	else
	{
		Buffs.Add(SkillBuff);
		BuffWidget = CreateWidget<UBuffWidget>(GetWorld(), LoadClass<UBuffWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/Blueprints/UI/WBP_Buff.WBP_Buff_C'")));
		
		if (!BuffWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("BuffWidget NULL"));
			return nullptr;
		}

		BuffWidget->BuffTexture = SkillBuff->BuffIcon; 
		//UHorizontalBox
		MainUserWidget->BuffBox->AddChildToHorizontalBox(BuffWidget);
		return BuffWidget;
		
	}
}


void ARBaseCharacter::RemoveBuff(ABuffSkill* SkillBuff)
{
	if (Buffs.Contains(SkillBuff))
	{
		Buffs.Remove(SkillBuff);
		BuffWidget->RemoveFromParent();
	}
}


//给装备设置Owner   有Owner   就不产生交互
void ARBaseCharacter::OnInteractionComPBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInterationInterface * InterationInterface =Cast<IInterationInterface>(OtherActor);
	if (InterationInterface)
	{
		InterationInterface->OnEnterPlayerRadius(this);
	}
}

void ARBaseCharacter::OnInteractionComEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInterationInterface* InterationInterface = Cast<IInterationInterface>(OtherActor);
	if (InterationInterface)
	{
		InterationInterface->OnLeavePlayerRadius(this);
	}
}

void ARBaseCharacter::InteractionToNPC()
{
	//创建碰撞的数组
	TArray<AActor*>OverLapActors;
	InterationComponent->GetOverlappingActors(OverLapActors,TSubclassOf<ABsaeNPC>());
	for (AActor* OverLapActor:OverLapActors)
	{
		IInterationInterface*IT=Cast<IInterationInterface>(OverLapActor);
		if (IT)
		{	
			IT->OnIteractWith(this);
		}
	}
}



void ARBaseCharacter::TestCompleteQuset()
{

	QuestManager->CurrentQuestActors[0]->OnSubGoalCompleted(0,true);
}

void ARBaseCharacter::TestFaileQusest()
{
	QuestManager->CurrentQuestActors[0]->OnSubGoalCompleted(1, false);
}



void ARBaseCharacter::OnShiftPressed()
{
	bShiftDown=true;
}

void ARBaseCharacter::OnShiftReleased()
{
	bShiftDown=false;
}


//装备武器
bool ARBaseCharacter::EquipItem(class AItemStaff* ItemStaff)
{
	if (ItemStaff)
	{

		if (!Staff)
		{
			if (InventoryRef->RemoveItemAtIndex(ItemStaff->Index,1))
			{
				Staff=ItemStaff;
				Staff->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("WeaponSocket"));
				return true;
			} 
			else
			{
				return false;
			}
		}
		else
		{
			if(UnEquipItem())
			{
				EquipItem(ItemStaff);
				return true;
			}
			else
			{
				return false;
			}
		}

	}
	else
	{
		return false;
	}

}


bool ARBaseCharacter::UnEquipItem()
{
	if(Staff)
	{
		if(InventoryRef->AddItem(Staff->GetClass(),1)==0)
		{
			Staff->Destroy();
			Staff=nullptr;

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}



void ARBaseCharacter::OnOverloaded()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed/2;
	OverloadParticleComp->Activate();
}

void ARBaseCharacter::OnOverloadedEnd()
{
	GetCharacterMovement()->MaxWalkSpeed=DefaultSpeed;
	OverloadParticleComp->Deactivate();
}


//金币
void ARBaseCharacter::IncreaseCoin(int Amount)
{
	if (Amount>0)
	{
		CurrentCoin+=Amount;
		MainUserWidget->InventoryWidget->UpdateCoinText(CurrentCoin);

		if(MainUserWidget->ShopWidget)
		{ 
			MainUserWidget->ShopWidget->UpdatCoin();
		}
	}
}

void ARBaseCharacter::DecreaseCoin(int Amount)
{
	if (Amount > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("DecreaseCoin(int Amount) %d"), Amount);

		CurrentCoin=FMath::Clamp(CurrentCoin-Amount,0, CurrentCoin);
		//UE_LOG(LogTemp, Warning, TEXT("DecreaseCoin(int Amount) %d"), Amount);
		MainUserWidget->InventoryWidget->UpdateCoinText(CurrentCoin);

		if (MainUserWidget->ShopWidget)
		{
			MainUserWidget->ShopWidget->UpdatCoin();
		}
	}
}




void ARBaseCharacter::ChangeCurrentHP(float DeltHP)
{
	CurrentHp =FMath::Clamp(  CurrentHp + DeltHP ,0.0f, TotalHp);
	//更新到UMG
	MainUserWidget->SetHPProgressBar(CurrentHp / TotalHp);
	/*UE_LOG(LogTemp, Warning, TEXT("CurrentHp %f  TotalHp%f "), CurrentHp, TotalHp);
	MainUserWidget->SetHPProgressBar(0.3);*/

}

void ARBaseCharacter::ChangeCurrentMP(float DeltMP)
{
	CurrentMp = FMath::Clamp(CurrentMp + DeltMP ,0.0f, TotalMp);
	MainUserWidget->SetMPProgressBar(CurrentMp / TotalMp);
}

void ARBaseCharacter::IncreaseChangeCurrentExp(float DeltExp)
{
	CurrentExp += DeltExp;

	if (CurrentExp>= NeededExpToNextLeve)
	{
		IncreaseLeveL();
		CurrentExp -= NeededExpToNextLeve;
	}
	MainUserWidget->SetExpPanl(CurrentExp, NeededExpToNextLeve);

	//MainUserWidget->SetLevelText(FText::AsNumber(CurrentExp));
}



void ARBaseCharacter::BeginPlayChangeCurrentExp()
{
	NeededExpToNextLeve = FMath::FloorToInt((FMath::Pow(CurrentLevel - 1, 3) + 60) / 5 * ((CurrentLevel - 1) * 2 + 60) + 60);
	MainUserWidget->SetExpPanl(CurrentExp, NeededExpToNextLeve);
}








void ARBaseCharacter::TEST01()
{
	TArray<int> TestArray;
	TestArray.Add(88);
	TestArray.Add(1);
	TestArray.Add(114);
	UE_LOG(LogTemp, Warning, TEXT("%d"), TestArray[0]);
	UE_LOG(LogTemp, Warning, TEXT("%d"),TestArray.Num());
}
