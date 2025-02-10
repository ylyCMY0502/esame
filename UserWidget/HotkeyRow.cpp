// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/HotkeyRow.h"
#include "UserWidget/SkillHotkey.h"
#include "Components/HorizontalBox.h"

TArray< USkillHotkey*> UHotkeyRow::GenerateHotkeys()
{
	for (auto Key : Keys)
	{
		USkillHotkey * SkillHotkey =CreateWidget<USkillHotkey>(GetWorld(),LoadClass<USkillHotkey>(GetWorld(), TEXT("WidgetBlueprint'/Game/Blueprints/UI/WBP_SkillHotkey.WBP_SkillHotkey_c'")));
		//设置按键
		SkillHotkey->SetKey(Key);
		//添加到排的数组
		SlotWidgets.Add(SkillHotkey);
		
		//UHorizontalBox
		//创建到排UHorizontalBox
		RowBox->AddChildToHorizontalBox(SkillHotkey);
	
	
	}
	
	return SlotWidgets;
}
