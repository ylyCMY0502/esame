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
	//����
	static float CaculateFinalDamage(float BaseDamage,int CritChance,TSubclassOf<ABaseElement>AttackerElement, TSubclassOf<ABaseElement>DefenderElement);
	
	//�ж��ǲ�����
	static bool BIsEnemy(AActor* Actor);


	//ö��ת�ַ���FText
	template<typename TEnum>
	static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value);

	//ö��ת����
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