// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryGameInstance.h"

void UBatteryGameInstance::SetBatteryValue(int32 NewValue)
{
	if (BatteryValue != NewValue)
	{
		BatteryValue = NewValue;
		
		OnBatteryValueChanged.Broadcast(BatteryValue);
	}
}
