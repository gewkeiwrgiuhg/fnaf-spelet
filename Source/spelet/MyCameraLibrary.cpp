// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraLibrary.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"


AActor* UMyCameraLibrary::GetCameraByName(UObject* WorldContextObject, const FString TargetName)
{
	UE_LOG(LogTemp, Error, TEXT(">>> GetCameraByName CALLED <<<"));
	
	UWorld* World = nullptr;

	if (WorldContextObject)
	{
		World = WorldContextObject->GetWorld();
	}

	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO WORLD"));
		return nullptr;
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(
		World,
		AActor::StaticClass(),
		Actors
	);

	for (AActor* Actor : Actors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *Actor->GetName());
	}

	for (AActor* Actor : Actors)
	{
		if (!Actor)
		{
			UE_LOG(LogTemp, Warning, TEXT("ACTOR NOT FOUND"));
			continue;
		};
		
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *Actor->GetName());
		if (Actor->GetName().Contains(TargetName)) //&& Actor->FindComponentByClass<UCameraComponent>())
		{
			return Actor;
		}
	}

	return nullptr;
}