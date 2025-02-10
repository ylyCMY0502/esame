// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Skill/SkillEnum.h"
#include "InputCoreTypes.h"
#include "Interface/DamageableInterface.h"
#include "RBaseCharacter.generated.h"

class ABaseElement;
class ABaseSkill;
class UTexture2D;
class ABuffSkill;
class UBuffWidget;



//技能阶段（技能等级属性）
USTRUCT()
struct FSkillStage
{
	GENERATED_USTRUCT_BODY()
	//技能等级
	UPROPERTY(EditAnywhere,Category="SkillDate")
	int RequiedLevel;
	//需要前置的技能
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	TArray<TSubclassOf<ABaseSkill>> RequiedSkills; 
	//新技能图片
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	UTexture2D* OverrideIcon; 	
	//技能颜色
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	FLinearColor Color;  
	//技能范围
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	float Rage; 
	//技能伤害
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	float Damage; 
	//伤害类型（物理还是魔法）
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	EDamageTypeSelected DamageType;
	//暴击率
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	int CriticalChance;
	//冷却时间
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	float CooldownTime;
	//释放时间
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	float CastingTime;
	//魔法消耗值
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	float ManaCost;
	//远程技能速度
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	float MissileSpeed;//远程技能速度

};


//技能的性息
USTRUCT()
struct FSkillInfo
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	FText Name;
	
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	UTexture2D* Icon;

	//关于技能的描述
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	FText Description;          

	//该技能属于哪个元素
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	TSubclassOf<ABaseElement>Element;

	//作用对象 （技能类型）
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	ETargetType Target;

	//结构体 技能拥有的阶段 通过FSkillInfol来获取FSkillStage  蓝图力手动添加
	UPROPERTY(EditAnywhere, Category = "SkillDate")
	TArray<FSkillStage>Stages;


};




UCLASS()
class RPGPROJECT_API ARBaseCharacter : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//设置移动
	void MoveForward(float Value);
	void MoveRight(float Value);
	//生成贴花
	void OnSetDestinationPressed();
	//移动生成贴花上
	void SetNewMoveDestination(const FVector Pos);

	//移动到光标，（内有选择敌人高亮显示血条）
	void MoveToCursor();

	//取消移动到鼠标
	void CancleMoveToCursor();

	//相机缩放
	void CameraZoomIn();
	void CameraZoomOut();

	//绑定显示技能的
	void ToogleShowSkillTree();
	//绑定任务
	void ToogleShowQuest();
	//背包
	void ToogleShowInventroy();
	//显示合成
	void ToogleShowCrafting();

	//读取数据
	void ReadDate();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	

	//重写 接口 函数
	virtual void OnReceiveDamage(float BaseDamage, EDamageTypeSelected Type, TSubclassOf<ABaseElement>Element, int CritChance, AActor* Attacker, ABaseSkill* Skill) override;

	//添加摄像机组建
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* RCamera;
	//添加弹簧组件
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* RSpringArm;
	//用于任务的胶囊组件
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* InterationComponent;



	//添加AMouseCursor类在编辑窗口
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMouseCursor> SpawnMouseCursor;

	class AMouseCursor* CurrentMouseCursor;

	//是不是鼠标在控制
	bool bHasMouseMoveCommand = false;

	UPROPERTY(EditAnywhere)
	float MidCameraZoom = 75;
	UPROPERTY(EditAnywhere)
	float MaxCameraZoom = 1500;
	UPROPERTY(EditAnywhere)
	float CameraZoomSpeed = 25;


	//玩家属性
	UPROPERTY(VisibleAnywhere, Category = "PalyerInfo")
		FString CharacterName;



	UPROPERTY(VisibleAnywhere, Category = "PalyerInfo")
		float CurrentHp;
	UPROPERTY(VisibleAnywhere, Category = "PalyerInfo")
		float CurrentMp;
	UPROPERTY(VisibleAnywhere, Category = "PalyerInfo")
		float CurrentExp;
	UPROPERTY(VisibleAnywhere, Category = "PalyerInfo")
		float CurrentLevel;



	//人物头像组建
	UPROPERTY(EditAnywhere, Category = "Portrait")
	class USceneCaptureComponent2D *PortraitComponent;


	//用户设置按键
	UPROPERTY(EditAnywhere, Category = "UI")
	TArray<FKey> Keys;
	UPROPERTY(EditAnywhere, Category = "UI")
	uint8 KeysPerRow;


	//技能**************************
	//初始技能
	//UPROPERTY(EditAnywhere, Category = "Skill")
	//TArray<TSubclassOf<ABaseSkill>> StartingSkills;

	
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ABaseElement>Element;

	//生成初始技能
	//void GenerateStartingSkills();

	//按键收集
	void OnAnyKeyPressed(FKey Key);
	
	//用于静止 连续按压
	bool bCanFindKey = true;

	//判断当前技能释放
	bool bIsCasting;

	//之前鼠标点击的AActor
	AActor* SelectedActor;

	//打断技能释放
	void CancleMissle();
	//升级
	void IncreaseLeveL();

	//下一级 要的经验值
	UPROPERTY(VisibleAnywhere)
	float NeededExpToNextLeve;

	float DefaultSpeed;


public:

	UPROPERTY(VisibleAnywhere, Category = "PalyerInfo")
		float TotalHp = 100;
	UPROPERTY(VisibleAnywhere, Category = "PalyerInfo")
		float TotalMp = 100;

	//玩家控制器
	class APlayerController* RPlayerController;

	//学习过的技能（）
	TArray<TSubclassOf<ABaseSkill>> LearntSkills;

	//当前技能  蓝图用到了 通过调用获得ASkillNova 下的OnSkillNotify
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	ABaseSkill* CurrentSkill;

	//主UI
	UPROPERTY(BlueprintReadOnly)
	class UMainUserWidget* MainUserWidget;

	//选择的敌人
	class ANormalEnemy* SelectedEnemy;

	//用于判断是不是 AMissileSkill 的技能攻击
	class AMissileSkill* MissileSkill;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//内联 
	
	//更新ARBaseCharacter  HP
	void ChangeCurrentHP(float DeltHP);
	FORCEINLINE float GetCurrentHP() { return CurrentHp; }
	FORCEINLINE float GetTotalHp() { return TotalHp; }
	FORCEINLINE void SetCurrentHP(float Hp) { CurrentHp = Hp; }

	//更新ARBaseCharacter  MP
	void ChangeCurrentMP(float DeltMP);
	FORCEINLINE float GetCurrentMP() { return CurrentMp; }
	FORCEINLINE float GetTotalMp() { return TotalMp; }
	FORCEINLINE void SetCurrentMP(float Mp) { CurrentMp= Mp; }

	FORCEINLINE float GetCurrentExp() { return CurrentExp; }
	FORCEINLINE void SetCurrentExp(float Exp) { CurrentExp= Exp; }
	//更新ARBaseCharacter  Exp
	void IncreaseChangeCurrentExp(float DeltExp);
	
	void BeginPlayChangeCurrentExp();

	FORCEINLINE float GetDefaultSpeed() { return DefaultSpeed; }
	FORCEINLINE void SetDefaultSpeed(float Speed) {  DefaultSpeed= Speed; }

	//设置ARBaseCharacter名字
	FORCEINLINE FString GetCharacterName() { return CharacterName; }
	//FORCEINLINE void SetCharacterName(FString Name){ CharacterName =Name; }
	FORCEINLINE void SetCharacterName(FString Name) {CharacterName = Name;}
	//FORCEINLINE FString GetCharacterName() { return CharacterName; }

	//获取  判断当前技能释放
	FORCEINLINE bool GetBIsCasting() { return bIsCasting; }

	//释放技能
	void BeginSpellCast(ABaseSkill* Skill);
	//结束技能释放
	void EndSpellCast(ABaseSkill* Skill);

	FORCEINLINE int GetCurrentLevel() {return CurrentLevel; }

	FORCEINLINE void SetCurrentLevel(int Level) { CurrentLevel= Level; }


	//Buff UI
	//创建Buff的数组
	TArray<ABuffSkill*>Buffs;
	UBuffWidget* BuffWidget;

	//添加Buff
	UBuffWidget* AddBuff(ABuffSkill*SkillBuff);
	//移除Buff
	void RemoveBuff(ABuffSkill* SkillBuff);


	//技能树 组件
	UPROPERTY(VisibleAnywhere)
	class USkillTreeComponent* SKillTreeCom;


	//任务系统界面
	UPROPERTY(EditAnywhere,Category="Quest")
	TSubclassOf<class AQuestManager> QuestManagerClass;
	//UPROPERTY(EditAnywhere, Category = "Quest")
	//class AQuestManager* TSTTTTTTT;
	
	UPROPERTY(BlueprintReadOnly)
	class AQuestManager* QuestManager;


	//靠近
	UFUNCTION()
	void OnInteractionComPBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//离开
	UFUNCTION()
	void OnInteractionComEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//用于E键交互
	void InteractionToNPC();

	//任务测试
	void TestCompleteQuset();
	void TestFaileQusest();



	//任务UI      蓝图重写 C++可不写     将会去执行蓝图中的程序
	UFUNCTION(BlueprintImplementableEvent)
	void PlaySideOutAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAddNewQuestAnimation();


	//*******捡起的道具*************
	TArray<TSubclassOf< class ABaseQuestProps> >ObtaninedPorps;

	UPROPERTY(BlueprintReadOnly)
	class  AInventory* InventoryRef;
	bool bShiftDown;
	class AStorage* StorageRef;
	void OnShiftPressed();
	void OnShiftReleased();

	//装备-------------
	class AItemStaff* Staff;
	bool EquipItem(class AItemStaff* ItemStaff);

	UFUNCTION(BlueprintCallable)
	bool UnEquipItem();
	//--------------

	UPROPERTY(EditAnywhere,Category="Item")
	class UParticleSystemComponent* OverloadParticleComp;
	void OnOverloaded();
	void OnOverloadedEnd();

	//金币
	int CurrentCoin = 0;
	void IncreaseCoin(int Amount);
	void DecreaseCoin(int Amount);

	class ARPGGameState* RPGGameState;

protected:
	UPROPERTY(EditAnywhere ,Category = "Inventory")
	TSubclassOf<class AInventory> InventoryClass;

	//UPROPERTY(EditAnywhere, Category = "SaveGame")
	//TSubclassOf<class ARPGGameState> RPGGameStateClass;

	UPROPERTY(EditAnywhere, Category = "Item")
	TArray<FKey> ItemKeys;
	











	UFUNCTION(BlueprintCallable)
	void TEST01();


};
