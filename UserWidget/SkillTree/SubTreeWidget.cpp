// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillTree/SubTreeWidget.h"
#include "Components/SkillTreeComponent.h"
#include "UserWidget/SkillTree/SkillTreeEntryWidget.h"
#include "UserWidget/SkillTree/ConnectionWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void USubTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GenerateContent();
}

void USubTreeWidget::GenerateContent()
{
	//清理
	Canvas->ClearChildren();
	SkillEntries.Empty();
	Connections.Empty();

	//根据数组中的技能个数 生成UI
	for (FSubTreeEntry Entry : Content.Skills)
	{
		USkillTreeEntryWidget * SkillTreeEntryWidget =CreateWidget<USkillTreeEntryWidget>(GetWorld(), LoadClass<USkillTreeEntryWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UI/SkillTree/WBP_SkillTreeEntry.WBP_SkillTreeEntry_c'")));
		SkillTreeEntryWidget->SubTree = this;
		SkillTreeEntryWidget->SkillClass = Entry.SpellClass;
		SkillEntries.Add(SkillTreeEntryWidget);

		//将UI加入画板 并返回
		UCanvasPanelSlot* TempSlot = Canvas->AddChildToCanvas(SkillTreeEntryWidget);
		TempSlot->SetPosition(Entry.Position);
		//显示级别
		TempSlot->SetZOrder(2);
		TempSlot->SetAutoSize(true);

	}


	//根据数组中line 生成UI
	for (FSubTreeConnection Connection : Content.Connections)
	{
		UConnectionWidget* ConnectionWidget = CreateWidget<UConnectionWidget>(GetWorld(), LoadClass<UConnectionWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/UI/SkillTree/WBP_Connection.WBP_Connection_c'")));
		ConnectionWidget->ForSpell = Connection.ForSpell;

		Connections.Add(ConnectionWidget);

		UCanvasPanelSlot* TempSlot = Canvas->AddChildToCanvas(ConnectionWidget);
		TempSlot->SetPosition(Connection.Position);
		TempSlot->SetSize(Connection.Size);
		
		//旋转
		ConnectionWidget->SetRenderTransform(Connection.Transfrom);
	}

}
