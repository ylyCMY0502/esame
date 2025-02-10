// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputCoreTypes.h"
#include "ItemHotkeyWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;
UCLASS()
class RPGPROJECT_API UItemHotkeyWidget : public UUserWidget
{
	GENERATED_BODY()


		


	
	//virtual bool Initialize()override;

public:
	//按键 每个按键对应一个key  用于释放SKillHotKEY
	//UPROPERTY(BlueprintReadOnly)
	FKey Key;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmountText;

	//图标
	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;

	//拖拽用
	UPROPERTY(meta = (BindWidget))
	UImage* Base;

	//快捷键是不是空
	bool bEmpty=true;
	//在那个插槽上 插槽上对应的索引
	int InvnetoryIndex;

	class AInventory* Inventory;
	FLinearColor DefaultTint =FLinearColor(0,0,0,0);
	FLinearColor DragOverTint = FLinearColor(0.1f, 0.2f, 0.03f, 0.35f);

	//图标是不是被拖动
	bool bDraggedOver;


	void UPdate();

	//按键的名字
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HotkeyName;




	//冷却时间
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CooldownText;

	

	//冷却旋转材质图标
	UPROPERTY(meta = (BindWidget))
	UImage* CooldownImage;



	//触发按键
	UPROPERTY(meta = (BindWidget))
	UButton* SkillButton;



	//*****鼠标拖拽*****
	UPROPERTY(EditAnywhere, Category = "DragDrop")
	TSubclassOf<UDragDropOperation> SkillDragOperation;


	//材质实例
	class UMaterialInstanceDynamic* CooldownMat;

	//快捷键是不是在激活中
	bool bDeactived;

protected:
	


	


	//*****鼠标拖拽*****
	//重写 只有鼠标右键按下的时候，才能执行检测的方法
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//当检查到拖拽 是调用
	//virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	//拖拽在另一UI上时
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	//拖拽UI是离开触发 
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	//当放下的时候
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

public:
	

	void Init(FKey Key,class AInventory* Inventory);


	




	////激活该快捷键，点击有效
	//void EnableHotkey();
	////快捷键失效
	//void DisableHotkey();



	
};
