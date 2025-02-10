// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SkillDragOperation.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USkillDragOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	//��ק���ŵļ���
	class USkillHotkey* FromHotkey;
	//��ǰ��ק�ļ���
	class ABaseSkill* SkillActor;
	
};
