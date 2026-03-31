// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryGameInstance.h"
#include "AICharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "NightWidget.h"
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
	float batterySubtraction = batteryDepletionRate * BatteryDepletionTime * batteryDepletionMultiplier;
	BatteryValue = FMath::Clamp(BatteryValue - batterySubtraction, 0.0f, 100.0f);
	OnBatteryValueChanged.Broadcast(BatteryValue);
}

void UBatteryGameInstance::ChangeBatteryDepletionTime(float newValue)
{
	batteryDepletionMultiplier = FMath::Clamp(batteryDepletionMultiplier + newValue, 1.0f, 100.0f);
}

void UBatteryGameInstance::AddTime()
{
	Time += 1;
	if (Time > 12)
	{
		UE_LOG(LogTemp, Warning, TEXT("Time: &d"), Time);
		Time = 1;
	}
	UE_LOG(LogTemp, Warning, TEXT("Time: &d"), Time);

	OnTimeChanged.Broadcast(Time);

	if (Time == WinTime)
	{
		NextNight();
	}
}
void UBatteryGameInstance::NextNight()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->GetPawn())
	{
		PC->SetIgnoreLookInput(true);
		PC->SetIgnoreMoveInput(true);
	}
	

	Night += 1;
	Night = FMath::Clamp(Night, 1, 5);
	if (Night >= 5)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("WinLevel"));
	}

	
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UNightWidget::StaticClass(), false);
	if (FoundWidgets.Num() > 0)
	{
		UNightWidget* NightWidget = Cast<UNightWidget>(FoundWidgets[0]);
		if (NightWidget)
		{
			NightWidget->PlayNightCutscene(Night); // pass your night int
		}
	}

	ResetEverything();
}

void UBatteryGameInstance::DisableAllLights()
{
	TArray<AActor*> lights;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Light", lights);

	for (AActor* Light : lights)
	{
		ALight* pointLight = Cast<ALight>(Light);
		if (pointLight)
		{
			pointLight->GetLightComponent()->SetVisibility(false);
		}
	}
}

void UBatteryGameInstance::ResetEverything()
{
	BatteryValue = 100;
	gameStarted = false;
	DisableAllLights();
	Time = 12;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->GetPawn())
	{
		TArray<AActor*> SpawnPoints;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), "SpawnPoint", SpawnPoints);
		if (SpawnPoints.Num() > 0)
		{
			PC->GetPawn()->SetActorLocation(SpawnPoints[0]->GetActorLocation());
		}

		PC->SetIgnoreLookInput(false);
		PC->SetIgnoreMoveInput(false);
	}

	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UWB_Objective::StaticClass(), false);
	if (FoundWidgets.Num() > 0)
		ObjectiveWidget = Cast<UWB_Objective>(FoundWidgets[0]);

	ObjectiveWidget->SetObjective(FText::FromString("Turn on the Power"));

	TArray<AActor*> Animatronics;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), Animatronics);

	for (AActor* Animatronic : Animatronics)
	{
		AAICharacter* AIChar = Cast<AAICharacter>(Animatronic);
		if (AIChar)
		{
			AIChar->AILevel += AILevelIncreasePerNight;
			AIChar->hasStarted = false;
			AIChar->currentWaypoint = 0;
		}
	}

}

void UBatteryGameInstance::EnableAllLights()
{
	TArray<AActor*> lights;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Light", lights);

	for (AActor* Light : lights)
	{
		ALight* pointLight = Cast<ALight>(Light);
		if (pointLight)
		{
			pointLight->GetLightComponent()->SetVisibility(true);
		}
	}
}
