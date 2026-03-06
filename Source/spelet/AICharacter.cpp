// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AAICharacter::StartAI()
{
	if (GetWorld() && refreshTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			MovementTimerHandle,
			this,
			&AAICharacter::AttemptMove,
			refreshTime,
			true
		);

		UE_LOG(LogTemp, Warning, TEXT("%s AI started"), *GetName());
	}
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	currentWaypoint = 0;
	
	if (MovementWaypoints.IsValidIndex(0) && MovementWaypoints[0].Waypoint != nullptr)
	{
		SetActorLocation(MovementWaypoints[0].Waypoint->GetActorLocation());
		SetActorRotation(MovementWaypoints[0].Waypoint->GetActorRotation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s saknar giltig start-waypoint!"), *GetName());
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
			FWaypointData waypointData = MovementWaypoints[currentWaypoint];
			AActor* targetWaypoint = waypointData.Waypoint;
			UAnimSequence* AnimToPlay = waypointData.AnimationToPlay;
			
			if (AnimToPlay)
			{
				GetMesh()->PlayAnimation(AnimToPlay, false);
				UE_LOG(LogTemp, Warning, TEXT("Playing animation: %s"), *AnimToPlay->GetName());
			}
			
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
		else // on final waypoint
		{
			if (alreadyOnFinalWaypoint == true)
			{
				UE_LOG(LogTemp, Warning, TEXT("AI is already on final waypoint, not setting new waypoint"));
				return;
			}
			
			if (finalWaypoint.Num() > 1)
			{
				// get a random waypoint to choose
				int32 RandomIndex = FMath::RandRange(0, finalWaypoint.Num() - 1);

				FWaypointData waypointData = finalWaypoint[RandomIndex];

				if (waypointData.Waypoint == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("Random final waypoint was null"));
					return;
				}

				alreadyOnFinalWaypoint = true;

				SetActorLocation(waypointData.Waypoint->GetActorLocation());
				SetActorRotation(waypointData.Waypoint->GetActorRotation());

				if (waypointData.AnimationToPlay)
				{
					GetMesh()->PlayAnimation(waypointData.AnimationToPlay, false);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No animation found for random final waypoint"));
				}
			}
			else
			{
				if (finalWaypoint[0].Waypoint == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("Final waypoint not found"));
					return;
				}
			
				alreadyOnFinalWaypoint = true;
			
				UE_LOG(LogTemp, Warning, TEXT("Is on final waypoint"));
				SetActorLocation(finalWaypoint[0].Waypoint->GetActorLocation());
				SetActorRotation(finalWaypoint[0].Waypoint->GetActorRotation());
			
				if (finalWaypoint[0].AnimationToPlay)
				{
					GetMesh()->PlayAnimation(finalWaypoint[0].AnimationToPlay, false);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("No animation found to play"));
				}
			}
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Roll failed, number gotten: %d vs AILevel: %d"), randomNumber, AILevel);
	}
}