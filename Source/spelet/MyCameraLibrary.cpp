// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraLibrary.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

ACameraActor* UMyCameraLibrary::GetCameraByName(UObject* WorldContextObject, FString targetName)
{
	if (!WorldContextObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO WCO"));
		return nullptr;
	}
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ACameraActor::StaticClass(), FoundActors);
	
	for (AActor* Actor : FoundActors)
	{
		if (Actor && (Actor->GetName() == targetName || Actor->GetActorLabel() == targetName))
		{
			return Cast<ACameraActor>(Actor);
		}
	}
	
	return nullptr;
}
