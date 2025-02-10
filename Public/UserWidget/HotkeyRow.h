// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(disable: 4458)

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HotkeyRow.generated.h"

/**
 * 
 */
class UHorizontalBox;
class USkillHotkey;

UCLASS()
class RPGPROJECT_API UHotkeyRow : public UUserWidget
{
	GENERATED_BODY()

	/// 存放  一排 USkillNotkey
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* RowBox;

	//按键的数组  外部传入 创建后对其赋值
	TArray<FKey> Keys;

	//一排拥有的USkillNotkey
	TArray< USkillHotkey*> SlotWidgets;


public:
	FORCEINLINE void SetHotkey(TArray<FKey> Keys) { this->Keys = Keys; }

	//生成Hotkeys
	TArray< USkillHotkey*> GenerateHotkeys();

};
