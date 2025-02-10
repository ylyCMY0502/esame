// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillTree/CategoryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "UserWidget/SkillTree/MainTreeWidget.h"

void UCategoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CategoryNameText->SetText(CategoryName);
	CategoryButton->OnHovered.AddDynamic(this, &UCategoryWidget::OnCategoryButtonHoverd);
	CategoryButton->OnUnhovered.AddDynamic(this, &UCategoryWidget::OnCategoryButtonUnhoverd);
	CategoryButton->OnClicked.AddDynamic(this, &UCategoryWidget::OnCategoryButtonClicked);
}

void UCategoryWidget::OnCategoryButtonHoverd()
{
	CategoryNameText->SetColorAndOpacity(FLinearColor(0.8f, 0.8f, 0.8f, 1));
}

void UCategoryWidget::OnCategoryButtonUnhoverd()
{
	if (! bSelected)
	{
		CategoryNameText->SetColorAndOpacity(FLinearColor(0.6f, 0.6f, 0.6f, 1));
	}
	
}

void UCategoryWidget::OnCategoryButtonClicked()
{
	//UMainTreeWidget
	MainTreeRef->OnCategoryClicked(Index);
}

void UCategoryWidget::ChangeState(bool bSelected)
{
	if (bSelected != this->bSelected)
	{
		this->bSelected = bSelected;

		FLinearColor TempColor;
		if (bSelected)
		{
			TempColor = FLinearColor::White;
		}
		else
		{
			TempColor = FLinearColor(0.6f, 0.6f, 0.6f, 1);
		}

		CategoryButton->SetIsEnabled(!bSelected);
	
		CategoryNameText->SetColorAndOpacity(TempColor);
		
		
	}
}
