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
		if (!Actor)
		{
			continue;
		}
		
		if (Actor->GetName().Contains(TargetName)) //&& Actor->FindComponentByClass<UCameraComponent>())
		{
			return Actor;
		}
	}

	return nullptr;
}

void UMyCameraLibrary::SwitchToCamera(UObject* WorldContextObject, const FString& actorName, bool inputEnabled, bool antiAliasingEnabled)
{
	AActor* camera = UMyCameraLibrary::GetCameraByName(WorldContextObject, actorName);
	if (camera)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
		PC->SetViewTarget(camera);
		PC->SetIgnoreLookInput(inputEnabled);
		PC->SetIgnoreMoveInput(inputEnabled);
		
		antiAliasingEnabled ? PC->ConsoleCommand("r.DefaultFeature.AntiAliasing 1") : PC->ConsoleCommand("r.DefaultFeature.AntiAliasing 0");
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO CAMERA FOUND"));
	}
}

void UMyCameraLibrary::SwitchToActorOfClass(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, bool inputEnabled, bool antiAliasingEnabled)
{
	AActor* camera = UGameplayStatics::GetActorOfClass(WorldContextObject, ActorClass);
	if (camera)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
		PC->SetViewTarget(camera);
		PC->SetIgnoreLookInput(!inputEnabled); // inverterad för att om inputEnabled är true så kommer input att vara disasbled
		PC->SetIgnoreMoveInput(!inputEnabled);
		
		antiAliasingEnabled ? PC->ConsoleCommand("r.DefaultFeature.AntiAliasing 1") : PC->ConsoleCommand("r.DefaultFeature.AntiAliasing 0");
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO CAMERA FOUND"));
	}
}
