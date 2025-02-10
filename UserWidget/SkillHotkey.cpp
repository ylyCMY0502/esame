// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillHotkey.h"
#include "Components/TextBlock.h"
#include "Character/Skill/BaseSkill.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UserWidget/SkillDrag.h"
#include "Blueprint/DragDropOperation.h"
#include "UserWidget/SkillDragOperation.h"




bool USkillHotkey::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	//�󶨰������
	SkillButton->OnClicked.AddDynamic(this, &USkillHotkey::OnSKillButtonClicked);

	//UMaterialInstanceDynamic
	CooldownMat = CooldownImage->GetDynamicMaterial();

	//��Ĭ������Ϊ���ɼ�
	SkillIcon->SetVisibility(ESlateVisibility::Hidden);
	CooldownText->SetVisibility(ESlateVisibility::Hidden);
	CooldownImage->SetVisibility(ESlateVisibility::Hidden);
	
	HotkeyName->Font.Size = 8;

	DragOverColor = FLinearColor(0.1f, 0.2f, 0.03f, 0.35f);
	DefautlColor = FLinearColor(0,0,0,0);
	return true;
}


void USkillHotkey::OnSKillButtonClicked()
{
	AssigendSpell->OnTryCastSpell();

}


void USkillHotkey::ResetStyle()
{
	bDraggedOver = false;
	Base->SetColorAndOpacity(DefautlColor);
}


FReply USkillHotkey::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//�����ڷ����˼��ܺ�
	if (AssigendSpell)
	{
		//��ȡ�ǲ�������Ҽ�����            �����ǲ�������ȴ��
		if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && !AssigendSpell->GetBOnCooldown())
		{
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton).NativeReply;
		}
		else
		{
			return FReply::Handled();
		}
	}
	else
	{
		return FReply::Handled();
	}

}


void USkillHotkey::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent,OutOperation);
	
	//����UI
	USkillDrag* SkillDrag = CreateWidget<USkillDrag>(GetWorld(), LoadClass<USkillDrag>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UI/WBP_SkillDrag.WBP_SkillDrag_c'")));

	if (AssigendSpell->GetCurrentStage().OverrideIcon)
	{
		SkillDrag->SetSkillTexture(AssigendSpell->GetCurrentStage().OverrideIcon);
	}
	else
	{
		SkillDrag->SetSkillTexture(AssigendSpell->GetSkillInfo().Icon);
	}

	
	UDragDropOperation* TmepOp = UWidgetBlueprintLibrary::CreateDragDropOperation(SkillDragOperation);
	TmepOp->DefaultDragVisual = SkillDrag;
	OutOperation = TmepOp;

	//USkillDragOperation* TmepSkillOp =Cast<USkillDragOperation>(TmepOp);

	

	

	
	if (Cast<USkillDragOperation>(OutOperation))
	{
		
		Cast<USkillDragOperation>(OutOperation)->SkillActor = AssigendSpell;
		Cast<USkillDragOperation>(OutOperation)->FromHotkey = this;
	}
}


bool USkillHotkey::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	
	
	//InOperation ����ק������  OutOperation
	if (Cast<USkillDragOperation>(InOperation))
	{
		
		if (!bDraggedOver && Cast<USkillDragOperation>(InOperation)->FromHotkey != this && AssigendSpell == nullptr)
		{
		
			Base->SetColorAndOpacity(DragOverColor);
			bDraggedOver = true;

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

void USkillHotkey::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	
	if (bDraggedOver)
	{

		if (Cast<USkillDragOperation>(InOperation))
		{
			//�ָ���ԭ����û�м��ܵ�����
			ResetStyle();
		}
	}
}

bool USkillHotkey::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry,  InDragDropEvent,  InOperation);
	if (Cast<USkillDragOperation>(InOperation)->FromHotkey != this)
	{
		if (AssigendSpell)
		{
			if (AssigendSpell->GetBOnCooldown())
			{
				return true;
			}
			else
			{
				ABaseSkill* LocalAAssigendSpell = AssigendSpell;
				//�����������
				ClearAssigendSpell();
				if (Cast<USkillDragOperation>(InOperation)->FromHotkey)
				{
					Cast<USkillDragOperation>(InOperation)->FromHotkey->ClearAssigendSpell();
					AssigenSpell(Cast<USkillDragOperation>(InOperation)->SkillActor);
					Cast<USkillDragOperation>(InOperation)->FromHotkey->AssigenSpell(LocalAAssigendSpell);
				}
				else
				{
					AssigenSpell(Cast<USkillDragOperation>(InOperation)->SkillActor);
				}
				

				ResetStyle();

				return true;

			}
		}
		else
		{
			if (Cast<USkillDragOperation>(InOperation)->FromHotkey)
			{
				Cast<USkillDragOperation>(InOperation)->FromHotkey->ClearAssigendSpell();
			}
		
			AssigenSpell(Cast<USkillDragOperation>(InOperation)->SkillActor);

			return true;
		}
	}
	else
	{
		return false;
	}

}

void USkillHotkey::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);



	if (!bHasSetKeyName)
	{
		HotkeyName->SetText(Key.GetDisplayName());

		bHasSetKeyName = true;

	
	}
}






void USkillHotkey::AssigenSpell(ABaseSkill* NewAssigendSpell)
{
	this->AssigendSpell = NewAssigendSpell;
	
	if (AssigendSpell)
	{
		UE_LOG(LogTemp, Warning, TEXT("AssigenSpell-Hotkey%d"), AssigendSpell->GetHotkey());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AssigenSpell-Hotkey NULL"));
	}
	
	if (!AssigendSpell) { return; }

	//����ABaseSkill�İ󶨰��� 
	AssigendSpell->SetHotkey(this);
	
	SkillButton->SetIsEnabled(true);

	if (AssigendSpell->GetCurrentStage().OverrideIcon)
	{
		//UImage
		SkillIcon->SetBrushFromTexture(AssigendSpell->GetCurrentStage().OverrideIcon);
	}
	else
	{
		SkillIcon->SetBrushFromTexture(AssigendSpell->GetSkillInfo().Icon);
	}

	//��ͼ�����ÿɼ����ɵ��
	SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}



void USkillHotkey::ClearAssigendSpell()
{
	if (AssigendSpell)
	{
		AssigendSpell->SetHotkey(nullptr);
		AssigendSpell=nullptr;

		SkillButton->SetIsEnabled(false);
		SkillIcon->SetBrushFromTexture(nullptr);

		//���ص�
		SkillIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}


//�����ݼ�
void USkillHotkey::EnableHotkey()
{
	bDeactived = false;
	if (!AssigendSpell->GetBOnCooldown())
	{
		SkillButton->SetIsEnabled(true);
		SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CooldownImage->SetVisibility(ESlateVisibility::Hidden);
		SkillIcon->SetColorAndOpacity(FLinearColor::White);
	}
}


void USkillHotkey::DisableHotkey()
{
	bDeactived = true;
	if (!AssigendSpell->GetBOnCooldown())
	{
		SkillButton->SetIsEnabled(false);
		SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
		CooldownImage->SetVisibility(ESlateVisibility::Visible);
	
		
		CooldownMat->SetScalarParameterValue(FName("Percent"), 1.0f);
	}
}
