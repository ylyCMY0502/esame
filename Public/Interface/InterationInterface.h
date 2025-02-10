// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterationInterface.generated.h"

class ARBaseCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGPROJECT_API IInterationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnEnterPlayerRadius( ARBaseCharacter* Character) = 0;
	virtual void OnLeavePlayerRadius(ARBaseCharacter* Character) = 0;
	//°´¼ü½»»¥
	virtual void OnIteractWith(ARBaseCharacter* Character) = 0;


};
