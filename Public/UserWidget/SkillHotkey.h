// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(disable: 4458)

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputCoreTypes.h"
#include "SkillHotkey.generated.h"

/**
 * 
 */
class UTextBlock;
class UImage;
class UButton;
class ABaseSkill;

UCLASS()
class RPGPROJECT_API USkillHotkey : public UUserWidget
{
	GENERATED_BODY()

	
	
	//按键 每个按键对应一个key  用于释放SKillHotKEY
	//UPROPERTY(BlueprintReadOnly)
	FKey Key;
	

	//加载一次时候用
	bool bHasSetKeyName= false;

	virtual bool Initialize()override;

public:
	//按键的名字
	UPROPERTY(meta = (BindWidget))
		UTextBlock* HotkeyName;

	//冷却时间
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CooldownText;

	//技能图标
	UPROPERTY(meta = (BindWidget))
	UImage* SkillIcon;

	//冷却旋转材质图标
	UPROPERTY(meta = (BindWidget))
	UImage* CooldownImage;

	//拖拽用
	UPROPERTY(meta = (BindWidget))
	UImage* Base;

	//触发按键
	UPROPERTY(meta = (BindWidget))
	UButton* SkillButton;

	//图标是不是被拖动
	bool bDraggedOver;
	//图标拖动过来的事，改变的颜色
	FLinearColor DragOverColor;
	//默认颜色
	FLinearColor DefautlColor;

	//*****鼠标拖拽*****
	UPROPERTY(EditAnywhere,Category="DragDrop")
	TSubclassOf<UDragDropOperation> SkillDragOperation;


	//材质实例
	class UMaterialInstanceDynamic* CooldownMat;

	//快捷键是不是在激活中
	bool bDeactived;

protected:
	//指定分配的技能 （创建技能）
	UPROPERTY(BlueprintReadOnly)
	ABaseSkill* AssigendSpell;

	//技能点击事件
	UFUNCTION()
	void OnSKillButtonClicked();

	//重设置为初始的样子
	void ResetStyle();


	//*****鼠标拖拽*****
	//重写 只有鼠标右键按下的时候，才能执行检测的方法
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//当检查到拖拽 是调用
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	//拖拽在另一UI上时
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	//拖拽UI是离开触发 
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	//当放下的时候
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

public:
	//绑定key 最终了来源ARBaseCharacter 的角色上
	FORCEINLINE void SetKey(FKey Key) {this->Key = Key; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FKey GetKey() {return Key;}

	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//绑定技能（分配技能）
	void AssigenSpell(ABaseSkill* NewAssigendSpell);

	//清除技能
	void ClearAssigendSpell();

	//激活该快捷键，点击有效
	void EnableHotkey();
	//快捷键失效
	void DisableHotkey();

	//获取分配的技能
	FORCEINLINE ABaseSkill* GetAssigendSpell() { return AssigendSpell; }

};
