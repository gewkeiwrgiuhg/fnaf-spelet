// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BatteryGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPELET_API UBatteryGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battery")
	int32 BatteryValue = 100;
};
