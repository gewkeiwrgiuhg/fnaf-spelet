// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "GameFramework/Actor.h"
#include "GameStartTrigger.generated.h"

UCLASS()
class SPELET_API AGameStartTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameStartTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	AAICharacter* AIToActivate;
	
	UPROPERTY(EditAnywhere)
	AActor* Wall1;

	UPROPERTY(EditAnywhere)
	AActor* Wall2;
};
