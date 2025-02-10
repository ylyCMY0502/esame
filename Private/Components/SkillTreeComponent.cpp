// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillTreeComponent.h"
#include "Character/RBaseCharacter.h"
#include "Character/Skill/BaseSkill.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/SkillTree/MainTreeWidget.h"
#include "UserWidget/SkillTree/SubTreeWidget.h"
#include "UserWidget/SkillTree/ConnectionWidget.h"
#include "UserWidget/SkillTree/SkillTreeEntryWidget.h"

// Sets default values for this component's properties
USkillTreeComponent::USkillTreeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter=Cast<ARBaseCharacter>(GetOwner());

	
}


// Called every frame
void USkillTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

//�����ж�ǰ�ü�����û��ѧϰ
bool USkillTreeComponent::BPlayerLearnedSpell(TSubclassOf<ABaseSkill>Skill)
{
	return PlayerCharacter->LearntSkills.Contains(Skill);
}

void USkillTreeComponent::AddSkillPoints(int Amount)
{
	SkillPoints += Amount;
}

bool USkillTreeComponent::BCanUpgradeSpell(ABaseSkill* Spell)
{
	//1 ���㼼�ܵ�   2����ȼ�����   3���ܳ���ԭ���ĵ������磨5/4��
	if (SkillPoints >0 && PlayerCharacter->GetCurrentLevel()>= Spell->GetNextStage().RequiedLevel  && Spell->GetCurrentStageIndex() < Spell->GetSkillInfo().Stages.Num()-1)
	{
		bool LocalBool = true;
		for (TSubclassOf<ABaseSkill>PreSkill : Spell->GetNextStage().RequiedSkills)
		{
			if (!BPlayerLearnedSpell(PreSkill))
			{
				//ǰ�ü���û��ѧϰ�귵��False
				LocalBool = false;
				break;
			}
		}

		return LocalBool;
	}
	else
	{
		return false;
	}
}


bool USkillTreeComponent::BSkillOnHotkey(ABaseSkill* Spell)
{
	bool LocalBool = false;
	for (USkillHotkey*TempHotkey : PlayerCharacter->MainUserWidget->GetAllHotkeySlots())
	{
		if (TempHotkey->GetAssigendSpell() && TempHotkey->GetAssigendSpell()==Spell)
		{
			LocalBool = true;
			break;
		}
	}
	return LocalBool;
}

void USkillTreeComponent::UpgradeSpell(ABaseSkill* Skill, USkillTreeEntryWidget* SkillTreeEntry)
{
	UpgradedSpell = Skill;
	this->EntryWidget = SkillTreeEntry;

	if (BCanUpgradeSpell(Skill))
	{
		SkillPoints -= 1;
		UpgradedSpell->ChangeCurrentStageIndex(1);

		
		if (UpgradedSpell->GetCurrentStageIndex()==0)
		{
			PlayerCharacter->LearntSkills.Add(UpgradedSpell->GetClass());
			EntryWidget->OnSpellLearnt();
			ActivateConnertions(Skill->GetClass());

			EntryWidget->UpdateStagesText();
			EntryWidget->UpdataIcon();
			MainTreeWidget->UpdateSp();
			UpdataAllEntries();
		}
		else
		{
			EntryWidget->UpdateStagesText();
			EntryWidget->UpdataIcon();
			MainTreeWidget->UpdateSp();
			UpdataAllEntries();
		}
		
	}
}

void USkillTreeComponent::DowngradeSpell(ABaseSkill* Skill, USkillTreeEntryWidget* SkillTreeEntry)
{
	this->UpgradedSpell = Skill;
	this->EntryWidget = SkillTreeEntry;

	UpgradedSpell->ChangeCurrentStageIndex(-1);
	SkillPoints += 1;
	EntryWidget->UpdateStagesText();
	EntryWidget->UpdataIcon();
	MainTreeWidget->UpdateSp();
	UpdataAllEntries();
	
}


void USkillTreeComponent::HanldShowCommand()
{

	if (bTreeSetup)
	{
		UE_LOG(LogTemp, Warning, TEXT("bTreeSetupTTT"));
		//�����ʾ������  ���ؾ���ʾ 
		if (PlayerCharacter->MainUserWidget->bTreeShown)
		{
		
			MainTreeWidget->SetVisibility(ESlateVisibility::Hidden);
			PlayerCharacter->MainUserWidget->bTreeShown = false;
			FInputModeGameAndUI InputMode;
			PlayerCharacter->RPlayerController->SetInputMode(InputMode);
		}
		else
		{
			MainTreeWidget->SetVisibility(ESlateVisibility::Visible);

			FInputModeUIOnly InputMode;
			PlayerCharacter->RPlayerController->SetInputMode(InputMode);
			PlayerCharacter->MainUserWidget->bTreeShown = true;
		}
	}
	
}



void USkillTreeComponent::SetupTree()
{
	MainTreeWidget = PlayerCharacter->MainUserWidget->SkillTree;

	if (MainTreeWidget)
	{
		MainTreeWidget->Initialize(this);
		bTreeSetup = true;
	}
}

void USkillTreeComponent::ActivateConnertions(TSubclassOf<ABaseSkill>ForSpell)
{
	for (USubTreeWidget*SubTreeWidget:MainTreeWidget->SubTreeWidgets)
	{
		for (UConnectionWidget* ConnectionWidget:SubTreeWidget->Connections)
		{
			if (ConnectionWidget->ForSpell == ForSpell)
			{
				ConnectionWidget->Activte();
			}
		}
	}
}

void USkillTreeComponent::UpdataAllEntries()
{
	for (USubTreeWidget* SubTreeWidget : MainTreeWidget->SubTreeWidgets)
	{
		for (USkillTreeEntryWidget* SkillTreeEntryWidget : SubTreeWidget->SkillEntries)
		{
			SkillTreeEntryWidget->UpdataUpgradeOverlay();
		}
	}
}

