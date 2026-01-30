// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyCameraLibrary.generated.h"

UCLASS()
class SPELET_API UMyCameraLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Camera", meta = (WorldContext = "WorldContextObject"))
    static AActor* GetCameraByName(UObject* WorldContextObject, const FString TargetName);
	
	UFUNCTION(BlueprintCallable, Category="Camera", meta=(WorldContext = "WorldContextObject"))
	static void SwitchToCamera(UObject* WorldContextObject, const FString& actorName, bool inputEnabled, bool antiAliasingEnabled);
};
