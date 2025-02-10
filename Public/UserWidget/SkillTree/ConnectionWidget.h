// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConnectionWidget.generated.h"

/**
 * ����������
 */
UCLASS()
class RPGPROJECT_API UConnectionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* LineDown;

	//ǰ�ü��ܵ�����
	TSubclassOf<class ABaseSkill>ForSpell;
	
	//�����ǲ��Ǳ�����
	bool bActivte;
	//���߼���
	void Activte();
	
};
