// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Skill/SkillTreeStruct.h"
#include "SubTreeWidget.generated.h"

/**
 * �ż������� �� ���ܵ�
 */
UCLASS()
class RPGPROJECT_API USubTreeWidget : public UUserWidget
{
	GENERATED_BODY()


	virtual void NativeConstruct()override;

public:

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Canvas;

	//��ż������߲�����Ϣ��
	FSubTreeContent Content;

	//�ż���UI
	TArray<class USkillTreeEntryWidget*>SkillEntries;
	//������UI
	TArray<class UConnectionWidget*>Connections;
	
	class UMainTreeWidget* MainTree;


	void GenerateContent();
};
