// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ElementInfo.generated.h"

/**
 *
 */
class ABaseElement;


//元素性息
USTRUCT()
struct FElementInfo

{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,Category="Element")
	FText Name;

	UPROPERTY(EditAnywhere, Category = "Element")
	class UTexture2D* Icon;    //元素图标

	UPROPERTY(EditAnywhere, Category = "Element")
	FLinearColor Color;    //元素颜色

	UPROPERTY(EditAnywhere, Category = "Element")
	TArray<TSubclassOf<ABaseElement>> Enhance;   //加强技能元素

	UPROPERTY(EditAnywhere, Category = "Element")
	TArray<TSubclassOf<ABaseElement>> Weakness;  //较弱技能元素

};
