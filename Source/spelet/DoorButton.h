// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "DoorButton.generated.h"

UCLASS()
class SPELET_API ADoorButton : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool isOpen = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	float OpenHeight = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	float MoveSpeed = 200.f;

	FVector InitialDoorLocation;
	FVector TargetDoorLocation;
	
	bool bIsMoving = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	AActor* DoorActor;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    virtual void Interact_Implementation() override;
};
