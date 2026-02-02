// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Custom Navigation")
	TArray<AActor*> MovementWaypoints;
	
	UFUNCTION(BlueprintCallable, Category="Actions")
	void AttemptMove();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float refreshTime = 0.5f;
	
	int32 currentWaypoint = 0;
	FTimerHandle MovementTimerHandle;
};