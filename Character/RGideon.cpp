// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RGideon.h"

ARGideon::ARGideon()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh>SkeMesh(TEXT("SkeletalMesh'/Game/ParagonGideon/Characters/Heroes/Gideon/Meshes/Gideon.Gideon'"));
	GetMesh()->SetSkeletalMesh(SkeMesh.Object);
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));
	GetMesh()->SetRelativeLocation(FVector(0,0,-88.f));



	static ConstructorHelpers::FClassFinder<UAnimInstance>Anim(TEXT("AnimBlueprint'/Game/Blueprints/Character/Animations/ABP_Gideon.ABP_Gideon_c'"));
	GetMesh()->SetAnimInstanceClass(Anim.Class);



}
