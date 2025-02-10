// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillTree/ConnectionWidget.h"
#include "Components/Image.h"

void UConnectionWidget::Activte()
{
	if (!bActivte)
	{
		bActivte = true;
		//UImage
		LineDown->SetOpacity(1);
	}
}
