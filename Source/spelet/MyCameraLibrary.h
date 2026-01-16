// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyCameraLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SPELET_API UMyCameraLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category="Camera")
	static ACameraActor* GetCameraByName(UObject* WorldContextObject, FString targetName);
};
