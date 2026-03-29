// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UBatteryGameInstance::SetBatteryValue(float NewValue)
{
	float Clamped = FMath::Clamp(NewValue, 0.0f, 100.0f);

	if (!FMath::IsNearlyEqual(BatteryValue, Clamped))
	{
		BatteryValue = Clamped;
		OnBatteryValueChanged.Broadcast(BatteryValue);
	}
}

void UBatteryGameInstance::SubtractBatteryValue()
{
	float batterySubtraction = batteryDepletionRate * BatteryDepletionTime;
    BatteryValue = FMath::Clamp(FMath::FloorToFloat((BatteryValue - batterySubtraction) * 10.f) / 10.f, 0.0f, 100.0f);
	OnBatteryValueChanged.Broadcast(BatteryValue);
}

void UBatteryGameInstance::ChangeBatteryDepletionTime(float newValue)
{
	batteryDepletionMultiplier = FMath::Clamp(batteryDepletionMultiplier + newValue, 1.0f, 100.0f);
}

void UBatteryGameInstance::SetTime(int32 NewTime)
{
	if (Time == NewTime)
	{
		return;
	}
	
	Time = NewTime;
	OnTimeChanged.Broadcast(Time);
}

void UBatteryGameInstance::NextNight()
{
	Night += 1;
	Night = FMath::Clamp(Night, 1, 5);
	if (Night >= 5)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("WinLevel"));
	}
}
