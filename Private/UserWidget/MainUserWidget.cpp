// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/MainUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "UserWidget/HotkeyRow.h"
#include "Components/VerticalBox.h"
#include "UserWidget/SkillDragOperation.h"
#include "Components/CanvasPanel.h"
#include "UserWidget/Quest/QuestWidget.h"
#include "Quest/BaseQuest.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/ScrollBox.h"
#include "UserWidget/Quest/SubGoalWidget.h"
#include "UMG/Public/Components/Button.h"
#include "UserWidget/Quest/QuestJournalWidget.h"
#include "UserWidget/SkillTree/MainTreeWidget.h"
#include "Components/SkillTreeComponent.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UserWidget/Inventory/InventoryDragDropOperation.h"
#include "UserWidget/Inventory/ItemDragDropOperation.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "UserWidget/Inventory/ThrowWidget.h"
#include "UMG/Public/Components/WrapBox.h"
#include "InventorySystem/Inventory.h"
#include "InventorySystem/BaseItem.h"
#include "UserWidget/Inventory/ItemObtainWidget.h"
#include "UMG/Public/Components/Border.h"
#include "UserWidget/SettingWidget.h"
#include "UserWidget/Inventory/ItemHotkeyWidget.h"


bool UMainUserWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	//��
	LevelText =Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Level")));
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_HP")));
	MPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_MP")));

	QusetButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnQusetButtonClicked);
	SkillButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnSkillButtonClicked);
	InventoryButton->OnClicked.AddDynamic(this, &UMainUserWidget::OnInventoryButtonClicked);
	SettingButton->OnClicked.AddDynamic(this,&UMainUserWidget::OnSettingButtonClicked);

	//UCanvasPanel
	EnemyCanvasPanel->SetVisibility(ESlateVisibility::Hidden);

	return true;
}

void UMainUserWidget::GenerateItemHotkey(TArray<FKey > ItemKeysToGenerate)
{
	ItemHotkeyWidgets.Empty();
	ItemHotkeyWrapBox->ClearChildren();

	for (FKey Key: ItemKeysToGenerate)
	{
		UItemHotkeyWidget* ItemHotkeyWidget =CreateWidget<UItemHotkeyWidget>(GetWorld(),LoadClass<UItemHotkeyWidget>(GetWorld(),TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_ItemHotkey.WBP_ItemHotkey_C'")));
		ItemHotkeyWidget->Init(Key,Inventory);
		ItemHotkeyWidgets.Add(ItemHotkeyWidget);
		ItemHotkeyWrapBox->AddChild(ItemHotkeyWidget);
	}
}

//UTextBlock UProgressBar
void UMainUserWidget::SetLevelText(FText Text)
{
	LevelText->SetText(Text);
}

void UMainUserWidget::SetHPProgressBar(float Percent)
{
	HPProgressBar->SetPercent(Percent);
}

void UMainUserWidget::SetMPProgressBar(float Percent)
{
	MPProgressBar->SetPercent(Percent);
}



//���ɼ��ܰ���
void UMainUserWidget::GenerateHotkeys(TArray<FKey> KeysToGenerate, uint8 KeysPerRow)
{
	//���µ�
	TArray<FKey> LocalKeys = KeysToGenerate;
	//Ҫ���ɵ�
	TArray<FKey> LocalModifyKeys = KeysToGenerate;

	if (KeysToGenerate.Num() % KeysPerRow != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tarry==0 or KeysPerRow==0%d"), (KeysToGenerate.Num() % KeysPerRow));
		return;
	}

	//�������еļ� �� �����ĸ������ʱ
	if (LocalKeys.Num() == KeysPerRow)
	{
		UHotkeyRow* HotkeyRow = CreateWidget<UHotkeyRow>(GetWorld(), LoadClass<UHotkeyRow>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UI/WBP_HotkeyRow.WBP_HotkeyRow_c'")));
		HotkeyRow->SetHotkey(LocalKeys); //���ⲿ�� KEY ���ø��Լ�
		//UVerticalBox
		HotkeyRowContainer->AddChildToVerticalBox(HotkeyRow);

		//����һ�ŵļ���  �����б���
		AllHotkeySlots.Append(HotkeyRow->GenerateHotkeys());

	}
	else
	{ 
		LocalModifyKeys.SetNum(KeysPerRow);
		UHotkeyRow* HotkeyRow = CreateWidget<UHotkeyRow>(GetWorld(), LoadClass<UHotkeyRow>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UI/WBP_HotkeyRow.WBP_HotkeyRow_c'")));
		HotkeyRow->SetHotkey(LocalModifyKeys);
		HotkeyRowContainer->AddChildToVerticalBox(HotkeyRow);

		//����һ�ŵļ���  �����б���
		AllHotkeySlots.Append(HotkeyRow->GenerateHotkeys());
		
		//����Ĳ�Ҫ
		for (FKey Key : LocalModifyKeys)
		{
			if (LocalKeys.Contains(Key))
			{
				LocalKeys.Remove(Key);
			}

		}
		GenerateHotkeys(LocalKeys, KeysPerRow);
	}

}

bool UMainUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);


	if (Cast<USkillDragOperation>(InOperation))
	{
		if (Cast<USkillDragOperation>(InOperation)->FromHotkey)
		{
			Cast<USkillDragOperation>(InOperation)->FromHotkey->ClearAssigendSpell();
		}
		return true;
	}
	else if (Cast<UInventoryDragDropOperation>(InOperation))
	{
		
		Cast<UInventoryDragDropOperation>(InOperation)->WidgetToDrag->AddToViewport();
		
		
		Cast<UInventoryDragDropOperation>(InOperation)->WidgetToDrag->SetDesiredSizeInViewport(FVector2D(595.5,700));
		FVector2D TempOffsetPos =Cast<UInventoryDragDropOperation>(InOperation)->OffsetPos;
		Cast<UInventoryDragDropOperation>(InOperation)->WidgetToDrag->SetPositionInViewport(InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition())- TempOffsetPos,false);
		return true;
	}     
	else if(Cast<UItemDragDropOperation>(InOperation))
	{
		UInventorySlotWidget* Slot = Cast<UItemDragDropOperation>(InOperation)->DraggedSlot;
		if (Slot->Amount>0)
		{
		
			if (Slot->ItemInfo.bCanBeStacked && Slot->Amount > 1)
			{

				ThrowWidget->Update(Slot->SlotIndex);
				ThrowWidget->SetVisibility(ESlateVisibility::Visible);
				InventoryWidget->InventoryBox->SetIsEnabled(false);

			}
			else
			{
				InventoryWidget->InventoryRef->RemoveItemAtIndex(Slot->SlotIndex, 1);
			}

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




void UMainUserWidget::SetExpPanl(float CurrnetExp, float NeededExpToNextLeve)
{
	ExpBar->SetPercent(CurrnetExp / NeededExpToNextLeve);
	CurrnetExpText->SetText(FText::AsNumber(CurrnetExp));
	NeededExpToNextLeveText->SetText(FText::AsNumber(NeededExpToNextLeve));
}

//****�����õ� �ӿ�
void UMainUserWidget::PlayPopupAnimation_Implementation()
{

}


class UQuestWidget* UMainUserWidget::AddQuestToList(class ABaseQuest* Quest)
{


	
	if (Quest)
	{
			UQuestWidget* QuestWidget = CreateWidget<UQuestWidget>(GetWorld(), LoadClass<UQuestWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UI/Quest/WBP_QuestWidget.WBP_QuestWidget_c'")));
			QuestWidget->QuestManager = QuestManager;
			QuestWidget->AssignedQuest = Quest;
			
			/*��ʼ��*/		QuestWidget->QuestName->SetIsEnabled(false);
			QuestWidgets.Add(QuestWidget);


			UScrollBoxSlot* Temp = Cast<UScrollBoxSlot>(QuestList->AddChild(QuestWidget));
			//���ж���
			Temp->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			//�����ƶ���
			Temp->SetPadding(FMargin(0, 8, 0, 0));
			return QuestWidget;

	} 
	else
	{
		return nullptr;
	}
}

void UMainUserWidget::OnQusetButtonClicked()
{
	if (bQuestVisibling)
	{
		QuestJournal->SetVisibility(ESlateVisibility::Hidden);
		bQuestVisibling=false;
	}
	else
	{
		QuestJournal->SetVisibility(ESlateVisibility::Visible);
		bQuestVisibling = true;
	}
}

void UMainUserWidget::OnSkillButtonClicked()
{
	
	SkillTree->SkillTreeCom->HanldShowCommand();
}


void UMainUserWidget::OnInventoryButtonClicked()
{
	if (bInventoryVisibling)
	{
		
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		bInventoryVisibling = false;
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		bInventoryVisibling = true;
	}
}

void UMainUserWidget::OnSettingButtonClicked()
{
	
	if (bSettingVisibling)
	{
		SettingWidget->SetVisibility(ESlateVisibility::Hidden);
		bSettingVisibling = false;
	}
	else
	{
		SettingWidget->SetVisibility(ESlateVisibility::Visible);
		bSettingVisibling=true;
	}

}

void UMainUserWidget::AddItemToObtainQueue(TSubclassOf<class ABaseItem>ItemClass,int Amount)
{
	if (! ObtainItemQueue.IsEmpty())
	{
		//��Ϊ����������Ʒ ���Ǽ����µ�
		ObtainItemQueue.Enqueue(FInventorySlot{ ItemClass ,Amount});
	} 
	else
	{
		//Ϊ��˵������ û��
		UItemObtainWidget* TempItemObtainWidget=CreateWidget<UItemObtainWidget>(GetWorld(),LoadClass<UItemObtainWidget>(nullptr,TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_ItemObtain.WBP_ItemObtain_C'")));
		TempItemObtainWidget->Init(ItemClass,Amount,this);//******����
		ObtainContainer->AddChild(TempItemObtainWidget);
		ObtainItemQueue.Enqueue(FInventorySlot{ ItemClass ,Amount });//******/������������ pop()��Ҳ������
	}


}

void UMainUserWidget::OnObtainMessageEend()
{
	ObtainItemQueue.Pop();
	ObtainContainer->ClearChildren();

	//��Ϊ��������һ���ʹ���һ��
	if (! ObtainItemQueue.IsEmpty())
	{
		UItemObtainWidget* TempItemObtainWidget = CreateWidget<UItemObtainWidget>(GetWorld(), LoadClass<UItemObtainWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Inventory/WBP_ItemObtain.WBP_ItemObtain_C'")));
		
		FInventorySlot TempSlot;

		ObtainItemQueue.Peek(TempSlot);
		TempItemObtainWidget->Init(TempSlot.ItemClass,TempSlot.Amout,this);
		ObtainContainer->AddChild(TempItemObtainWidget);
	}
}

