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
	//���� ÿ��������Ӧһ��key  �����ͷ�SKillHotKEY
	//UPROPERTY(BlueprintReadOnly)
	FKey Key;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmountText;

	//ͼ��
	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;

	//��ק��
	UPROPERTY(meta = (BindWidget))
	UImage* Base;

	//��ݼ��ǲ��ǿ�
	bool bEmpty=true;
	//���Ǹ������ ����϶�Ӧ������
	int InvnetoryIndex;

	class AInventory* Inventory;
	FLinearColor DefaultTint =FLinearColor(0,0,0,0);
	FLinearColor DragOverTint = FLinearColor(0.1f, 0.2f, 0.03f, 0.35f);

	//ͼ���ǲ��Ǳ��϶�
	bool bDraggedOver;


	void UPdate();

	//����������
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HotkeyName;




	//��ȴʱ��
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CooldownText;

	

	//��ȴ��ת����ͼ��
	UPROPERTY(meta = (BindWidget))
	UImage* CooldownImage;



	//��������
	UPROPERTY(meta = (BindWidget))
	UButton* SkillButton;



	//*****�����ק*****
	UPROPERTY(EditAnywhere, Category = "DragDrop")
	TSubclassOf<UDragDropOperation> SkillDragOperation;


	//����ʵ��
	class UMaterialInstanceDynamic* CooldownMat;

	//��ݼ��ǲ����ڼ�����
	bool bDeactived;

protected:
	


	


	//*****�����ק*****
	//��д ֻ������Ҽ����µ�ʱ�򣬲���ִ�м��ķ���
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//����鵽��ק �ǵ���
	//virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	//��ק����һUI��ʱ
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	//��קUI���뿪���� 
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	//�����µ�ʱ��
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

public:
	

	void Init(FKey Key,class AInventory* Inventory);


	




	////����ÿ�ݼ��������Ч
	//void EnableHotkey();
	////��ݼ�ʧЧ
	//void DisableHotkey();



	
};
