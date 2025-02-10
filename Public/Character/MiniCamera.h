// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "MiniCamera.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AMiniCamera : public ASceneCapture2D
{
	GENERATED_BODY()

	AMiniCamera();

	virtual void BeginPlay() override;
	

	class ARBaseCharacter* Character;


public:
	//用了获取位置
	void OutsideTick();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	float MiniCameraHeight = 1800;



};
