// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillTree/SkillTreeEntryWidget.h"
#include "Character/Skill/BaseSkill.h"
#include "Character/RBaseCharacter.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UserWidget/SkillTree/SubTreeWidget.h"
#include "UserWidget/SkillTree/ConnectionWidget.h"
#include "UserWidget/SkillTree/MainTreeWidget.h"
#include "Components/SkillTreeComponent.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UserWidget/SkillDrag.h"
#include "UserWidget/SkillDragOperation.h"

void USkillTreeEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SkillActor = GetWorld()->SpawnActor<ABaseSkill>(SkillClass, FVector::ZeroVector, FRotator::ZeroRotator);
	SkillActor->SetPlayerReference(SubTree->MainTree->SkillTreeCom->PlayerCharacter);
	UpdateStagesText();
	UpdataIcon();
	UpdataUpgradeOverlay();
	PlusButton->OnClicked.AddDynamic(this, &USkillTreeEntryWidget::OnPlusButtonClicked);
	MinusButton->OnClicked.AddDynamic(this, &USkillTreeEntryWidget::OnMinusButtonClicked);
}

int USkillTreeEntryWidget::GetAmountOfStages()
{
	//ABaseSkill
	return SkillActor->GetSkillInfo().Stages.Num();
}

void USkillTreeEntryWidget::UpdateStagesText()
{
	CurrnetStageText->SetText(FText::AsNumber(SkillActor->GetCurrentStageIndex() + 1));
	StageText->SetText(FText::AsNumber(GetAmountOfStages()));

}



void USkillTreeEntryWidget::UpdataIcon()
{
	UTexture2D* TempTexture;

	if (SkillActor->GetCurrentStageIndex()<0)
	{
		TempTexture = SkillActor->GetSkillInfo().Icon;
	} 
	else
	{
		if (SkillActor->GetCurrentStage().OverrideIcon)
		{
			TempTexture = SkillActor->GetCurrentStage().OverrideIcon;
		}
		else 
		{
			TempTexture = SkillActor->GetSkillInfo().Icon;
		}
	}

	SkillIcon->SetBrushFromTexture(TempTexture);
}

void USkillTreeEntryWidget::UpdataUpgradeOverlay()
{
	//��ѧϰ����
	if (SubTree->MainTree->SkillTreeCom->BCanUpgradeSpell(SkillActor))
	{
		PlusButton->SetVisibility(ESlateVisibility::Visible);
		MinusButton->SetVisibility(ESlateVisibility::Hidden);
		UpgradeOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);//�����ɵ��

		//UE_LOG(LogTemp, Warning, TEXT("GetCurrentStageIndex%d"), SkillActor->GetCurrentStageIndex());

		if (SkillActor->GetCurrentStageIndex()>=1)
		{
			MinusButton->SetVisibility(ESlateVisibility::Visible);			
		} 
		//��������Ϊ��ɫ
		SetColorAndOpacity(FLinearColor::White);
	}
	//����ѧϰ����
	else
	{	
		UE_LOG(LogTemp, Warning, TEXT("----------SkillActor->GetCurrentStageIndex()---"));
		//����-
		PlusButton->SetVisibility(ESlateVisibility::Hidden);
		if (SkillActor->GetCurrentStageIndex() >= 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("----------"));
			MinusButton->SetVisibility(ESlateVisibility::Visible);
			UpgradeOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SetColorAndOpacity(FLinearColor::White);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("-----NO +++-----"));
			//ѧϰ�˼���+-������
			if (SubTree->MainTree->SkillTreeCom->BPlayerLearnedSpell(SkillClass))
			{
				UpgradeOverlay->SetVisibility(ESlateVisibility::Hidden);
				SetColorAndOpacity(FLinearColor::White);
			}
			else
			{
				//����ѧϰ��ɫ��
				SetColorAndOpacity(FLinearColor(1,1,1,0.5f));
				UpgradeOverlay->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		
		
	}
	
}



void USkillTreeEntryWidget::OnSpellLearnt()
{
	bSpellLearnt = true;
	SkillIcon->SetColorAndOpacity(FLinearColor::White);
}



void USkillTreeEntryWidget::OnPlusButtonClicked()
{
	SubTree->MainTree->SkillTreeCom->UpgradeSpell(SkillActor, this);
}

void USkillTreeEntryWidget::OnMinusButtonClicked()
{
	SubTree->MainTree->SkillTreeCom->DowngradeSpell(SkillActor, this);
}

FReply USkillTreeEntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//�жϼ����ǲ���ѧϰ�� ѧϰ�˾Ϳ�����ק
	if (bSpellLearnt)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton).NativeReply;
	}
	else
	{
		return FReply::Handled();
	}

}







void USkillTreeEntryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	//������û�в���ק
	if (!SubTree->MainTree->SkillTreeCom->BSkillOnHotkey(SkillActor))
	{
		//����UI
		USkillDrag* SkillDrag = CreateWidget<USkillDrag>(GetWorld(), LoadClass<USkillDrag>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UI/WBP_SkillDrag.WBP_SkillDrag_c'")));

		if (SkillActor->GetCurrentStage().OverrideIcon)
		{
			SkillDrag->SetSkillTexture(SkillActor->GetCurrentStage().OverrideIcon);
		}
		else
		{
			SkillDrag->SetSkillTexture(SkillActor->GetSkillInfo().Icon);
		}


		UDragDropOperation* TmepOp = UWidgetBlueprintLibrary::CreateDragDropOperation(SkillDragOperation);
		TmepOp->DefaultDragVisual = SkillDrag;
		OutOperation = TmepOp;

		//USkillDragOperation* TmepSkillOp =Cast<USkillDragOperation>(TmepOp);

		if (Cast<USkillDragOperation>(OutOperation))
		{

			Cast<USkillDragOperation>(OutOperation)->SkillActor = SkillActor;
			Cast<USkillDragOperation>(OutOperation)->FromHotkey = nullptr;
		}
	}
}
