// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CameraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPELET_API UCameraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* BatteryText;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UBorder* DoorsBorder;
};
