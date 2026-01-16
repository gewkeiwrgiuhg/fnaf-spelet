// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorButton.h"

// Sets default values
ADoorButton::ADoorButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADoorButton::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("INTERACTING WITH DOOR"));
	
	isOpen = !isOpen;
	bIsMoving = true;
	
	if (isOpen)
	{
		TargetDoorLocation = InitialDoorLocation + FVector(0.f, 0.f, OpenHeight);
	}
	else
	{
		TargetDoorLocation = InitialDoorLocation;
	}
}

// Called when the game starts or when spawned
void ADoorButton::BeginPlay()
{
	Super::BeginPlay();
	
	if (DoorActor)
	{
		InitialDoorLocation = DoorActor->GetActorLocation();
		TargetDoorLocation = InitialDoorLocation;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("door actor not found"));
	}
}

// Called every frame
void ADoorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsMoving)
	{
		FVector currentLocation = DoorActor->GetActorLocation();
		FVector newLocation = FMath::VInterpTo(currentLocation, TargetDoorLocation, DeltaTime, MoveSpeed);
		
		DoorActor->SetActorLocation(newLocation);
		
		if (newLocation.Equals(TargetDoorLocation, 1.f))
		{
			bIsMoving = false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("bIsMoving is false"));
	}
}