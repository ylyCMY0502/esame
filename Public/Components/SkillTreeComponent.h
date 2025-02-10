// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Skill/SkillTreeStruct.h"
#include "SkillTreeComponent.generated.h"

class ABaseSkill;
class USkillTreeEntryWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	class ARBaseCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere ,Category="Skill")
	int SkillPoints=2;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FSubTreeCategory>Categories;

	// Sets default values for this component's properties
	USkillTreeComponent();

protected:

	

	//技能树中的技能是不是已经设置好
	bool bTreeSetup;



	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	ABaseSkill* UpgradedSpell;
	class USkillTreeEntryWidget* EntryWidget;


	class UMainTreeWidget* MainTreeWidget;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//用于判断前置技能有没有学习
	bool BPlayerLearnedSpell(TSubclassOf<ABaseSkill>Skill);
	//添加点数
	void AddSkillPoints(int Amount);
	//能不能学习技能
	bool BCanUpgradeSpell(ABaseSkill* Spell);
	//用于判断技能在没在快捷栏中
	bool BSkillOnHotkey(ABaseSkill* Spell);
	
	//+-
	void UpgradeSpell(ABaseSkill* Skill, USkillTreeEntryWidget* SkillTreeEntry);
	void DowngradeSpell(ABaseSkill* Skill, USkillTreeEntryWidget* SkillTreeEntry);

	//操作显示命令
	void HanldShowCommand();

	void SetupTree();

	//激活连线
	void ActivateConnertions(TSubclassOf<ABaseSkill>ForSpell);
	void UpdataAllEntries();



};
