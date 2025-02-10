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

	/// ���  һ�� USkillNotkey
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* RowBox;

	//����������  �ⲿ���� ��������丳ֵ
	TArray<FKey> Keys;

	//һ��ӵ�е�USkillNotkey
	TArray< USkillHotkey*> SlotWidgets;


public:
	FORCEINLINE void SetHotkey(TArray<FKey> Keys) { this->Keys = Keys; }

	//����Hotkeys
	TArray< USkillHotkey*> GenerateHotkeys();

};
