// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NightWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPELET_API UNightWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayNightCutscene(int32 Night);
};
