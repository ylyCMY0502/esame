// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StaticLibrary.generated.h"

class ABaseSkill;
class ABaseElement;


UCLASS()
class RPGPROJECT_API UStaticLibrary : public UObject
{
	GENERATED_BODY()

public:
	//受伤
	static float CaculateFinalDamage(float BaseDamage,int CritChance,TSubclassOf<ABaseElement>AttackerElement, TSubclassOf<ABaseElement>DefenderElement);
	
	//判断是不敌人
	static bool BIsEnemy(AActor* Actor);


	//枚举转字符串FText
	template<typename TEnum>
	static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value);

	//枚举转数组
	template<typename TEnum>
	static FORCEINLINE TArray<TEnum>EnumGetList(const FString& Name);
};



template<typename TEnum>
static FORCEINLINE FString UStaticLibrary:: GetEnumValueAsString(const FString& Name, TEnum Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!EnumPtr)
	{
		return FString("InValid");
	}
	FString TempString = EnumPtr->GetNameByValue((int64)Value).ToString();
	TempString.RemoveFromStart(Name +"::");
	return TempString;
}


template<typename TEnum>
static FORCEINLINE TArray<TEnum>
UStaticLibrary::EnumGetList(const FString& Name)
{
	TArray<TEnum> Result;
	UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	for (int i = 0; i < pEnum->GetMaxEnumValue(); i++)
	{
		Result.Add(static_cast<TEnum> (i));
	}
	return Result;
}