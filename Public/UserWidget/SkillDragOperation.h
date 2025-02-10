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
	//拖拽后存放的技能
	class USkillHotkey* FromHotkey;
	//当前拖拽的技能
	class ABaseSkill* SkillActor;
	
};
