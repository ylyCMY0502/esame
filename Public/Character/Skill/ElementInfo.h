// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ElementInfo.generated.h"

/**
 *
 */
class ABaseElement;


//Ԫ����Ϣ
USTRUCT()
struct FElementInfo

{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,Category="Element")
	FText Name;

	UPROPERTY(EditAnywhere, Category = "Element")
	class UTexture2D* Icon;    //Ԫ��ͼ��

	UPROPERTY(EditAnywhere, Category = "Element")
	FLinearColor Color;    //Ԫ����ɫ

	UPROPERTY(EditAnywhere, Category = "Element")
	TArray<TSubclassOf<ABaseElement>> Enhance;   //��ǿ����Ԫ��

	UPROPERTY(EditAnywhere, Category = "Element")
	TArray<TSubclassOf<ABaseElement>> Weakness;  //��������Ԫ��

};
