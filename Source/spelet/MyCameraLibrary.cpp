// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraLibrary.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"


AActor* UMyCameraLibrary::GetCameraByName(UObject* WorldContextObject, const FString TargetName)
{
	if (!WorldContextObject) return nullptr;

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(
		WorldContextObject,
		AActor::StaticClass(),
		Actors
	);

	for (AActor* Actor : Actors)
	{
		if (!Actor) continue;

		if (Actor->GetName().Contains(TargetName) &&
			Actor->FindComponentByClass<UCameraComponent>())
		{
			return Actor;
		}
	}

	return nullptr;
}