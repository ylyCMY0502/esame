// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MouseCursor.h"
#include "Components/DecalComponent.h"
#include "Components/SceneComponent.h"
#include "Materials/Material.h"
#include "Components/BoxComponent.h"


// Sets default values
AMouseCursor::AMouseCursor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//������������������齨
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SursorSceneComponent"));
	
	SursorToMouseDecalWorld=CreateDefaultSubobject<UDecalComponent>(TEXT("SursorToMouseDecalWorld"));
	SursorToMouseDecalWorld->SetupAttachment(RootComponent);
	//��������
	SursorToMouseDecalWorld->SetRelativeRotation(FRotator(90.f, 0.f, 0.f).Quaternion());
	SursorToMouseDecalWorld->DecalSize=FVector(15.f,20.f,20.f);
	//Ĭ�ϼ�������
	ConstructorHelpers::FObjectFinder<UMaterial>SursorDecal(TEXT("Material'/Game/Materials/MouseCursor/M_Cursor_Decal.M_Cursor_Decal'"));
	if (SursorDecal.Succeeded())//�ж�SursorDecal�ǲ�����
	{
		SursorToMouseDecalWorld->SetDecalMaterial(SursorDecal.Object);
	}

	//�����ײBOX
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DBoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetBoxExtent(FVector(20.f, 25.f, 25.f));
	BoxComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f).Quaternion());
	
	//��Ӱ󶨺���
	//BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMouseCursor::OnOverlap);
}

// Called when the game starts or when spawned
void AMouseCursor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMouseCursor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//void AMouseCursor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	//UE_LOG(LogTemp, Warning, TEXT("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"));
//	
//	//����ǲ������� �Ǿ�����
//	if (OtherActor == GetOwner())
//	{
//		Destroy();
//	}
//}


//��ײ���   ������ײ�Զ�����
void AMouseCursor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	//����ǲ������� �Ǿ�����
	if (OtherActor == GetOwner())
	{
		Destroy();
	}
}


