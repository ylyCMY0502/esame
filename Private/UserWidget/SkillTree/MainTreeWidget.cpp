// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillTree/MainTreeWidget.h"
#include "Components/Button.h"
#include "Components/SkillTreeComponent.h"
#include "UserWidget/SkillTree/SubTreeWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UserWidget/SkillTree/CategoryWidget.h"
#include "Components/TextBlock.h"
#include "Character/RBaseCharacter.h"
#include "Components/ScrollBox.h"


#define LOCTEXT_NAMESPACE "UMainTreeWidget"

void UMainTreeWidget::NativeConstruct()
{
	//UButton
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this, &UMainTreeWidget::OnCloseButtonClicked);
}

void UMainTreeWidget::OnCategoryClicked(int Index)
{
	if (CurrentlySelectedIndex != Index)
	{
		if (CurrentlySelectedIndex==-1)
		{
			//UWidgetSwitcher
			//如果没有现在 显示窗口
			SubTreeSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			CurrentlySelectedIndex = Index;

			CategoryWidgets[CurrentlySelectedIndex]->ChangeState(true);
			SubTreeSwitcher->SetActiveWidgetIndex(CurrentlySelectedIndex);
		} 
		else
		{
			CategoryWidgets[CurrentlySelectedIndex]->ChangeState(false);
			CurrentlySelectedIndex = Index;
			CategoryWidgets[CurrentlySelectedIndex]->ChangeState(true);
			SubTreeSwitcher->SetActiveWidgetIndex(CurrentlySelectedIndex);
		}
	}
}

void UMainTreeWidget::OnCloseButtonClicked()
{

	SkillTreeCom->HanldShowCommand();
}

void UMainTreeWidget::UpdateLevel()
{

	LevelText->SetText(FText::Format(LOCTEXT("SubGoalWindget", "LV.{0}"),FText::AsNumber(SkillTreeCom->PlayerCharacter->GetCurrentLevel())));
}


void UMainTreeWidget::UpdateSp()
{
	 SPText->SetText(FText::AsNumber(SkillTreeCom->SkillPoints));
}
	
void UMainTreeWidget::GenerateCategories()
{
	SubTreeSwitcher->ClearChildren();
	CategoriesBox->ClearChildren();

	CategoryWidgets.Empty();
	SubTreeWidgets.Empty();

	for (int i=0;i<TreeCategories.Num();i++)
	{
		UCategoryWidget* CategoryWidget = CreateWidget<UCategoryWidget>(GetWorld(), LoadClass<UCategoryWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UI/SkillTree/WBP_Category.WBP_Category_C'")));
		CategoryWidget->CategoryName = TreeCategories[i].Name;
		CategoryWidget->Index = i;
		CategoryWidget->MainTreeRef = this;
		CategoryWidgets.Add(CategoryWidget);
		CategoriesBox->AddChild(CategoryWidget);//

		USubTreeWidget* SubTreeWidget = CreateWidget<USubTreeWidget>(GetWorld(), LoadClass<USubTreeWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UI/SkillTree/WBP_SubTree.WBP_SubTree_c'")));
		SubTreeWidget->Content = TreeCategories[i].Content;
		SubTreeWidget->MainTree = this;
		SubTreeWidgets.Add(SubTreeWidget);
		SubTreeSwitcher->AddChild(SubTreeWidget);
	}

}

void UMainTreeWidget::Initialize(class USkillTreeComponent* AssignedTreeComp)
{
	SkillTreeCom = AssignedTreeComp;
	TreeCategories = SkillTreeCom->Categories;
	UpdateLevel();
	UpdateSp();
	GenerateCategories();
	OnCategoryClicked(0);
}


#undef LOCTEXT_NAMESPACE 