#include "MyCameraLibrary.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

AActor* UMyCameraLibrary::GetCameraByName(UObject* WorldContextObject, const FString TargetName)
{
    UWorld* World = (WorldContextObject) ? WorldContextObject->GetWorld() : nullptr;
    if (!World) return nullptr;

    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), Actors);

    for (AActor* Actor : Actors)
    {
        if (Actor && Actor->GetName().Contains(TargetName))
        {
            
            return Actor;
            
        }
    }

    return nullptr;
}

void UMyCameraLibrary::SwitchToCamera(UObject* WorldContextObject, const FString& actorName, bool inputEnabled, bool antiAliasingEnabled)
{
    AActor* CameraActor = UMyCameraLibrary::GetCameraByName(WorldContextObject, actorName);
    APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

    if (CameraActor && PC)
    {
        PC->SetViewTarget(CameraActor);
        PC->SetIgnoreLookInput(!inputEnabled);
        PC->SetIgnoreMoveInput(!inputEnabled);
       
        FString Command = antiAliasingEnabled ? TEXT("r.DefaultFeature.AntiAliasing 1") : TEXT("r.DefaultFeature.AntiAliasing 0");
        PC->ConsoleCommand(Command);
    }
}

void UMyCameraLibrary::SwitchToActorOfClass(
    UObject* WorldContextObject,
    APawn* PawnToSwitch,
    bool inputEnabled,
    bool antiAliasingEnabled)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

    if (PawnToSwitch && PC)
    {
        PC->Possess(PawnToSwitch);

        PC->SetIgnoreLookInput(!inputEnabled);
        PC->SetIgnoreMoveInput(!inputEnabled);

        FString Command = antiAliasingEnabled
            ? TEXT("r.DefaultFeature.AntiAliasing 1")
            : TEXT("r.DefaultFeature.AntiAliasing 0");

        PC->ConsoleCommand(Command);
    }
}
