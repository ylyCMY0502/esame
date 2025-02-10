// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SelectableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USelectableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGPROJECT_API ISelectableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//选择上
	virtual void OnSelectable(class ARBaseCharacter* Character) = 0;
	//鼠标离开
	virtual void OnSelectionEnd(class ARBaseCharacter* Character) = 0;

};
