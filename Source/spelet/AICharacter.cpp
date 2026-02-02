// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	currentWaypoint = 0;
	
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AAICharacter::AttemptMove, refreshTime, true);
	if (MovementWaypoints.IsValidIndex(0) && MovementWaypoints[0] != nullptr)
	{
		SetActorLocation(MovementWaypoints[0]->GetActorLocation());
		SetActorRotation(MovementWaypoints[0]->GetActorRotation());
	}
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::AttemptMove()
{
	if (AILevel == 0) return;

	int32 randomNumber = FMath::RandRange(1,20);
	if (randomNumber <= AILevel) // kan flytta ai:n
	{
		if (currentWaypoint + 1 < MovementWaypoints.Num()) // nuvarande waypoint är inte högre än totala mängden waypoints
		{
			currentWaypoint++;
			AActor* targetWaypoint = MovementWaypoints[currentWaypoint];
			
			if (targetWaypoint != nullptr)
			{
				SetActorLocation(targetWaypoint->GetActorLocation());
				SetActorRotation(targetWaypoint->GetActorRotation());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("No target waypoint found."));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Roll failed, number gotten: %d vs AILevel: &d"), randomNumber, AILevel);
	}
}