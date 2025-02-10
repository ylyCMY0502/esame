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

	

	//�������еļ����ǲ����Ѿ����ú�
	bool bTreeSetup;



	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	ABaseSkill* UpgradedSpell;
	class USkillTreeEntryWidget* EntryWidget;


	class UMainTreeWidget* MainTreeWidget;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//�����ж�ǰ�ü�����û��ѧϰ
	bool BPlayerLearnedSpell(TSubclassOf<ABaseSkill>Skill);
	//��ӵ���
	void AddSkillPoints(int Amount);
	//�ܲ���ѧϰ����
	bool BCanUpgradeSpell(ABaseSkill* Spell);
	//�����жϼ�����û�ڿ������
	bool BSkillOnHotkey(ABaseSkill* Spell);
	
	//+-
	void UpgradeSpell(ABaseSkill* Skill, USkillTreeEntryWidget* SkillTreeEntry);
	void DowngradeSpell(ABaseSkill* Skill, USkillTreeEntryWidget* SkillTreeEntry);

	//������ʾ����
	void HanldShowCommand();

	void SetupTree();

	//��������
	void ActivateConnertions(TSubclassOf<ABaseSkill>ForSpell);
	void UpdataAllEntries();



};
