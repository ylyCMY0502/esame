// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/MiniMapWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Engine/Texture2D.h"

bool UMiniMapWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;;
	}
	else 
	{
		Global =LoadObject<UTexture2D>(nullptr,TEXT("/Script/Engine.Texture2D'/Game/Textures/Mini_Map/Minimap/mp_icon_map.mp_icon_map'"));
		Arrow= LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Textures/Mini_Map/MapArrow.MapArrow'"));
		SetGoalHitVisible(false);
		return true;
	}
	
}

void UMiniMapWidget::RotateDirection(float Angle)
{
	DirectionArrow->SetRenderTransformAngle(Angle);
}

void UMiniMapWidget::SetDistanceText(float Distance)
{
	DistanceText->SetText(FText::AsNumber(Distance));
}

void UMiniMapWidget::SetGoalHitVisible(bool bVisible)
{
	if (bVisible)
	{
		DirectionArrow->SetBrushFromTexture(Arrow);
		DistanceOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	} 
	else
	{
		DirectionArrow->SetBrushFromTexture(Global);
		DistanceOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
	

}
