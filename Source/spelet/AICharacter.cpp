#include "AICharacter.h"

AAICharacter::AAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAICharacter::StartAI()
{
	if (playerDied) return;

	UE_LOG(LogTemp, Warning, TEXT("%s StartAI called, AILevel = %d"), *GetName(), AILevel);	
	
	hasStarted = true;
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

	BindDoorDispatchers();
	UE_LOG(LogTemp, Warning, TEXT("AILevel (BeginPlay): %d"), AILevel);
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAICharacter::BindDoorDispatchers()
{
	for (AActor* Door : DoorActors)
	{
		if (Door == nullptr) continue;

		FMulticastDelegateProperty* DelegateProperty = FindFProperty<FMulticastDelegateProperty>(
			Door->GetClass(),
			TEXT("OnDoorStateChanged") // Must match your Blueprint dispatcher name exactly
		);

		if (DelegateProperty)
		{
			FScriptDelegate NewDelegate;
			NewDelegate.BindUFunction(this, TEXT("OnDoorStateChangedCallback"));

			FMulticastScriptDelegate* MulticastDelegate = DelegateProperty->ContainerPtrToValuePtr<FMulticastScriptDelegate>(Door);
			if (MulticastDelegate)
			{
				MulticastDelegate->Add(NewDelegate);
				UE_LOG(LogTemp, Warning, TEXT("Bound to door dispatcher on %s"), *Door->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not find OnDoorStateChanged dispatcher on %s"), *Door->GetName());
		}
	}
}

void AAICharacter::OnDoorStateChangedCallback()
{
	// Only care about door changes once AI is on final waypoint
	if (!alreadyOnFinalWaypoint) return;

	UE_LOG(LogTemp, Warning, TEXT("Door state changed while AI is on final waypoint, checking doors..."));
	CheckNearbyDoors();
}

void AAICharacter::CheckNearbyDoors()
{
	if (playerDied) return;

	bool bAnyDoorOpen = false;

	for (AActor* Door : DoorActors)
	{
		if (Door == nullptr) continue;

		FBoolProperty* BoolProp = FindFProperty<FBoolProperty>(
			Door->GetClass(),
			TEXT("isOpen") // Your blueprint boolean variable name
		);

		if (BoolProp && BoolProp->GetPropertyValue_InContainer(Door))
		{
			bAnyDoorOpen = true;
			UE_LOG(LogTemp, Warning, TEXT("Door %s is open! Player has %.1f seconds to close it."),
				*Door->GetName(), DoorCloseGraceTime);
		}
	}

	if (bAnyDoorOpen)
	{
		// Clear any existing timer before starting a new one
		GetWorld()->GetTimerManager().ClearTimer(DoorCheckTimerHandle);

		GetWorld()->GetTimerManager().SetTimer(
			DoorCheckTimerHandle,
			this,
			&AAICharacter::OnGraceTimerExpired,
			DoorCloseGraceTime,
			false
		);
	}
	else
	{
		// Door was closed in time, cancel the timer
		GetWorld()->GetTimerManager().ClearTimer(DoorCheckTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("All doors are closed, timer cancelled."));
	}
}

void AAICharacter::OnGraceTimerExpired()
{
	for (AActor* Door : DoorActors)
	{
		if (Door == nullptr) continue;

		FBoolProperty* BoolProp = FindFProperty<FBoolProperty>(
			Door->GetClass(),
			TEXT("isOpen") // Your blueprint boolean variable name
		);

		if (BoolProp && BoolProp->GetPropertyValue_InContainer(Door))
		{
			UE_LOG(LogTemp, Warning, TEXT("Grace period expired! Door %s is still open."), *Door->GetName());
			OnDoorNotClosed.Broadcast(); // Hook this up in Blueprint for your consequence
		}
	}
}

void AAICharacter::AttemptMove()
{
	if (playerDied)
	{
		GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
		return;
	}


	if (AILevel == 0) return;

	int32 randomNumber = FMath::RandRange(1,20);
	if (randomNumber <= AILevel)
	{
		if (currentWaypoint + 1 < MovementWaypoints.Num())
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
				int32 RandomIndex = FMath::RandRange(0, finalWaypoint.Num() - 1);
				FWaypointData waypointData = finalWaypoint[RandomIndex];

				if (waypointData.Waypoint == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("Random final waypoint was null"));
					return;
				}

				alreadyOnFinalWaypoint = true;
				CheckNearbyDoors(); // <-- Check doors on arrival

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
				CheckNearbyDoors(); // <-- Check doors on arrival

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
		UE_LOG(LogTemp, Warning, TEXT("Roll failed, number gotten: %d vs AILevel: %d. Current waypoint: %d!"), randomNumber, AILevel, currentWaypoint);
		UE_LOG(LogTemp, Warning, TEXT("%s AILevel (AttemptMove): %d"), *GetName(), AILevel);
	}
}