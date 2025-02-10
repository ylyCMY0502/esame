// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

class UTexture2D;
UCLASS()
class RPGPROJECT_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()


private:

	virtual bool Initialize()override;



	UPROPERTY(meta=(BindWidget))
	class UTextBlock* DistanceText;


protected:
	
	


public:



	UPROPERTY(meta = (BindWidget))
	class UImage* DirectionArrow;

	UPROPERTY(EditAnywhere)
	UTexture2D* Arrow;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Global;

	UPROPERTY(EditAnywhere)
	class UTexture* Test;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* DistanceOverlay;

	void RotateDirection(float Angle);

	void SetDistanceText(float Distance);

	void SetGoalHitVisible(bool bVisible);
};
