#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h" // <--- Add this include
#include "BatteryGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FBatteryValueChanged,
	float,
	NewBatteryValue
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FTimeChanged,
	int32,
	NewTime
	);

/**
 * */
UCLASS()
class SPELET_API UBatteryGameInstance : public UGameInstance
{
	GENERATED_BODY()
    
public:
	UPROPERTY(BlueprintAssignable, Category="Battery")
	FBatteryValueChanged OnBatteryValueChanged;

	UPROPERTY(BlueprintAssignable, Category="Time")
	FTimeChanged OnTimeChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battery")
	float BatteryValue = 100;

	UFUNCTION(BlueprintCallable, Category="Battery")
	void SetBatteryValue(float NewValue);
	
	UFUNCTION(BlueprintCallable, Category="Battery")
	void SubtractBatteryValue();
	
	UFUNCTION(BlueprintCallable, Category="Battery")
	void ChangeBatteryDepletionTime(float newValue);

	UPROPERTY(BlueprintReadWrite, Category="UI")
	UUserWidget* CameraUIRef;
	
	UPROPERTY(BlueprintReadWrite, Category="Battery")
	float batteryDepletionMultiplier = 1.f;
	
	UFUNCTION(BlueprintCallable, Category="Time")
	void SetTime(int32 NewTime);
	
	UFUNCTION(BlueprintCallable, Category="Time")
	void NextNight();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool gameStarted = false;
	
	UPROPERTY(BlueprintReadOnly, Category="Time", meta=(AllowPrivateAccess=true))
	int32 Night = 1;
	
	UPROPERTY(BlueprintReadWrite, Category="Battery", meta=(AllowPrivateAccess=true))
	float BatteryDepletionTime = 0.1f;
	
	UPROPERTY(BlueprintReadWrite, Category = "Battery")
	float batteryDepletionRate = 1.0f; // Enheter per sekund
private:
	UPROPERTY(BlueprintReadOnly, Category="Time", meta=(AllowPrivateAccess=true))
	int32 Time = 12;
};