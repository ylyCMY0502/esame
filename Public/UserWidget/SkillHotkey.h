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

	
	
	//���� ÿ��������Ӧһ��key  �����ͷ�SKillHotKEY
	//UPROPERTY(BlueprintReadOnly)
	FKey Key;
	

	//����һ��ʱ����
	bool bHasSetKeyName= false;

	virtual bool Initialize()override;

public:
	//����������
	UPROPERTY(meta = (BindWidget))
		UTextBlock* HotkeyName;

	//��ȴʱ��
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CooldownText;

	//����ͼ��
	UPROPERTY(meta = (BindWidget))
	UImage* SkillIcon;

	//��ȴ��ת����ͼ��
	UPROPERTY(meta = (BindWidget))
	UImage* CooldownImage;

	//��ק��
	UPROPERTY(meta = (BindWidget))
	UImage* Base;

	//��������
	UPROPERTY(meta = (BindWidget))
	UButton* SkillButton;

	//ͼ���ǲ��Ǳ��϶�
	bool bDraggedOver;
	//ͼ���϶��������£��ı����ɫ
	FLinearColor DragOverColor;
	//Ĭ����ɫ
	FLinearColor DefautlColor;

	//*****�����ק*****
	UPROPERTY(EditAnywhere,Category="DragDrop")
	TSubclassOf<UDragDropOperation> SkillDragOperation;


	//����ʵ��
	class UMaterialInstanceDynamic* CooldownMat;

	//��ݼ��ǲ����ڼ�����
	bool bDeactived;

protected:
	//ָ������ļ��� ���������ܣ�
	UPROPERTY(BlueprintReadOnly)
	ABaseSkill* AssigendSpell;

	//���ܵ���¼�
	UFUNCTION()
	void OnSKillButtonClicked();

	//������Ϊ��ʼ������
	void ResetStyle();


	//*****�����ק*****
	//��д ֻ������Ҽ����µ�ʱ�򣬲���ִ�м��ķ���
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//����鵽��ק �ǵ���
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	//��ק����һUI��ʱ
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	//��קUI���뿪���� 
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	//�����µ�ʱ��
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

public:
	//��key ��������ԴARBaseCharacter �Ľ�ɫ��
	FORCEINLINE void SetKey(FKey Key) {this->Key = Key; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FKey GetKey() {return Key;}

	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//�󶨼��ܣ����似�ܣ�
	void AssigenSpell(ABaseSkill* NewAssigendSpell);

	//�������
	void ClearAssigendSpell();

	//����ÿ�ݼ��������Ч
	void EnableHotkey();
	//��ݼ�ʧЧ
	void DisableHotkey();

	//��ȡ����ļ���
	FORCEINLINE ABaseSkill* GetAssigendSpell() { return AssigendSpell; }

};
