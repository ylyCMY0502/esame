// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillDrag.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"


void USkillDrag::SetSkillTexture(UTexture2D* Texture)
{
	
	SkillIcon->SetBrushFromTexture(Texture);

}
