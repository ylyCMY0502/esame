// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MiniCamera.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
#include "Character/RBaseCharacter.h"

AMiniCamera::AMiniCamera()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCaptureComponent2D()->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
}

void AMiniCamera::BeginPlay()
{
	Super::BeginPlay();

	Character=Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	SetActorLocation(FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, Character->GetActorLocation().Z + MiniCameraHeight));
}

void AMiniCamera::OutsideTick()
{
	//UE_LOG(LogTemp, Warning, TEXT("DSDSD"));
	SetActorLocation(FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, Character->GetActorLocation().Z+ MiniCameraHeight));
}

void AMiniCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OutsideTick();
}
