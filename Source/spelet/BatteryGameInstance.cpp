// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryGameInstance.h"

void UBatteryGameInstance::SetBatteryValue(int32 NewValue)
{
	if (BatteryValue != NewValue)
	{
		NewValue *= batteryDepletionMultiplier;
		BatteryValue = FMath::Clamp(NewValue,0,100);
		
		OnBatteryValueChanged.Broadcast(BatteryValue);
	}
}

void UBatteryGameInstance::SubtractBatteryValue()
{
	int32 batterySubtraction = FMath::RoundToInt(1.0f * batteryDepletionMultiplier);
	int32 NewValue = FMath::Clamp(BatteryValue - batterySubtraction, 0, 100);
    
	if (BatteryValue != NewValue)
	{
		BatteryValue = NewValue;
		OnBatteryValueChanged.Broadcast(BatteryValue);
	}
}

void UBatteryGameInstance::ChangeBatteryDepletionTime(float newValue)
{
	batteryDepletionMultiplier = FMath::Clamp(batteryDepletionMultiplier + newValue, 1.0f, 100.0f);
}