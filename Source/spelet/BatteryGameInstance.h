// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BatteryGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FBatteryValueChanged,
	int32,
	NewBatteryValue
);

/**
 * 
 */
UCLASS()
class SPELET_API UBatteryGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category="Battery")
	FBatteryValueChanged OnBatteryValueChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battery")
	int32 BatteryValue = 100;

	UFUNCTION(BlueprintCallable, Category="Battery")
	void SetBatteryValue(int32 NewValue);
	
	UPROPERTY(BlueprintReadWrite, Category="UI")
	class UWB_CameraUI* CameraUIRef;
};
