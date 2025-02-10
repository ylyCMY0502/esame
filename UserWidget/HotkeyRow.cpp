// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/HotkeyRow.h"
#include "UserWidget/SkillHotkey.h"
#include "Components/HorizontalBox.h"

TArray< USkillHotkey*> UHotkeyRow::GenerateHotkeys()
{
	for (auto Key : Keys)
	{
		USkillHotkey * SkillHotkey =CreateWidget<USkillHotkey>(GetWorld(),LoadClass<USkillHotkey>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UI/WBP_SkillHotkey.WBP_SkillHotkey_c'")));
		//���ð���
		SkillHotkey->SetKey(Key);
		//��ӵ��ŵ�����
		SlotWidgets.Add(SkillHotkey);
		
		//UHorizontalBox
		//��������UHorizontalBox
		RowBox->AddChildToHorizontalBox(SkillHotkey);
	
	
	}
	
	return SlotWidgets;
}
