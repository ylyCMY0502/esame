// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Quest/QuestListEntryWidget.h"
#include "Components/TextBlock.h"
#include "UserWidget/Quest/QuestJournalWidget.h"
#include "Quest/BaseQuest.h"
#include "Character/RBaseCharacter.h"
#include "UMG/Public/Components/Button.h"


void UQuestListEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Update();

	QuestButton->OnClicked.AddDynamic(this,&UQuestListEntryWidget::OnQuestButtonClickde);
}

void UQuestListEntryWidget::UpdateLevelColor()
{
	FLinearColor TempColor;
	if (Journal->PlayerCharacter->GetCurrentLevel() > AssignedQuest->QuestInfo.SuggestedLevel)
	{
		TempColor=FLinearColor::Green;
	} 
	else if (Journal->PlayerCharacter->GetCurrentLevel() == AssignedQuest->QuestInfo.SuggestedLevel)
	{	
		TempColor=FLinearColor::Blue;
	}
	else 
	{
		TempColor=FLinearColor::Red;
	}
	SuggestedLevel->SetColorAndOpacity(TempColor);
}


void UQuestListEntryWidget::Update()
{
	if (AssignedQuest->QuestInfo.Name.ToString().Len() < 20)
	{
		SetQuestNmae(AssignedQuest->QuestInfo.Name);
	}
	else
	{
		SetQuestNmae(FText::FromString(AssignedQuest->QuestInfo.Name.ToString().LeftChop(AssignedQuest->QuestInfo.Name.ToString().Len() - 17).Append("...")));
	}

	SetSuggestedLevel(FText::AsNumber(AssignedQuest->QuestInfo.SuggestedLevel));
	UpdateLevelColor();
	SetRegionName(FText::FromString(GetERegionsEnumString(AssignedQuest->QuestInfo.Region)));

}

FString UQuestListEntryWidget::GetERegionsEnumString(ERegions Region)
{
	const UEnum* EnumPtr= FindObject<UEnum>(ANY_PACKAGE,TEXT("ERegions"),true);
	if (!EnumPtr)
	{
		return FString("InValid");
	}
	FString TempString =EnumPtr->GetNameByValue((int64)Region).ToString();
	TempString.RemoveFromStart("ERegions::");
	return TempString;
}

void UQuestListEntryWidget::SetQuestNmae(FText Text)
{
	QuestNmae->SetText(Text);
}

void UQuestListEntryWidget::SetRegionName(FText Text)
{
	RegionName->SetText(Text);
}

void UQuestListEntryWidget::SetSuggestedLevel(FText Text)
{
	SuggestedLevel->SetText(Text);
}

void UQuestListEntryWidget::OnQuestButtonClickde()
{
	Journal->OnQuestClicked(this);
}
