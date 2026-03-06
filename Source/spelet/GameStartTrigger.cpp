// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartTrigger.h"

#include "PlayerCharacter.h"

// Sets default values
AGameStartTrigger::AGameStartTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OnActorBeginOverlap.AddDynamic(this, &AGameStartTrigger::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGameStartTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameStartTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameStartTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		if (AIToActivate)
		{
			AIToActivate->StartAI();
		}
	}
}