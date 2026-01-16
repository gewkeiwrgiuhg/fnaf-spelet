// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerSitCharacter.generated.h"

UCLASS()
class SPELET_API APlayerSitCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerSitCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;
	
	void Interact();
    	
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
    float InteractDistance = 300.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APlayerController* PC;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	float EdgeTurnSpeed = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	float DeadzoneThreshold = 0.2f;
};
