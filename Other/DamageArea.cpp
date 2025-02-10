// Fill out your copyright notice in the Description page of Project Settings.


#include "Other/DamageArea.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Character/RBaseCharacter.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADamageArea::ADamageArea()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->SetBoxExtent(FVector(100,100,100));

	//BoxComponent->SetRelativeScale3D(FVector(2,2,2));

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ADamageArea::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADamageArea::OnEndOverlap);
	
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystme"));
	ParticleSystem->SetupAttachment(BoxComponent);

	

}

void ADamageArea::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ARBaseCharacter>(OtherActor))
	{
		Player = Cast<ARBaseCharacter>(OtherActor);
		GetWorldTimerManager().SetTimer(TimerHandle_DamageArea,this,&ADamageArea::ApplayAreaDamage,1.0f,true);
	}
}

void ADamageArea::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ARBaseCharacter>(OtherActor))
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_DamageArea);
	}
}

void ADamageArea::ApplayAreaDamage()
{
	UGameplayStatics::ApplyDamage(Player, 5.0F, nullptr, this,nullptr);
}

