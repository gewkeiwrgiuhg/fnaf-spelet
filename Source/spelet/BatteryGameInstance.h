#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h" // <--- Add this include
#include "BatteryGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FBatteryValueChanged,
	int32,
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
	int32 BatteryValue = 100;

	UFUNCTION(BlueprintCallable, Category="Battery")
	void SetBatteryValue(int32 NewValue);
	
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
	
private:
	UPROPERTY(BlueprintReadOnly, Category="Time", meta=(AllowPrivateAccess=true))
	int32 Time = 12;
};