// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class UAnimSequence;

USTRUCT(BlueprintType)
struct FWaypointData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waypoints")
	AActor* Waypoint = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waypoints")
	UAnimSequence* AnimationToPlay = nullptr;
};

UCLASS()
class SPELET_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta = (ClampMin = "1", ClampMax = "20", UIMin = "1", UIMax = "20"))
	int32 AILevel = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	FString name = "Placeholder";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Custom Navigation")
	FWaypointData finalWaypoint; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Custom Navigation")
	TArray<FWaypointData> MovementWaypoints;
	
	UFUNCTION(BlueprintCallable, Category="Actions")
	void AttemptMove();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float refreshTime = 10.0f;
	
	int32 currentWaypoint = 0;
	FTimerHandle MovementTimerHandle;
};