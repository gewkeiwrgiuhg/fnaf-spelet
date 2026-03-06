#include "MyCameraLibrary.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Components/SpotLightComponent.h"
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

void UMyCameraLibrary::ResetAllLights(UObject* WorldContextObject)
{
    UWorld* World = (WorldContextObject) ? WorldContextObject->GetWorld() : nullptr;
    if (!World) return;
    
    TArray<AActor*> Cameras;
    UGameplayStatics::GetAllActorsOfClass(World, ACameraActor::StaticClass(), Cameras);
        
    for (AActor* Cam : Cameras)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Cam->GetName());
            
        TArray<USpotLightComponent*> Lights;
        Cam->GetComponents<USpotLightComponent>(Lights);

        for (USpotLightComponent* Light : Lights)
        {
            if (Light)
            {
                Light->SetVisibility(false);
            }
        }
    }
}

void UMyCameraLibrary::SwitchToCamera(UObject* WorldContextObject, const FString& actorName, bool inputEnabled, bool antiAliasingEnabled)
{
    UWorld* World = (WorldContextObject) ? WorldContextObject->GetWorld() : nullptr;
    if (!World) return;
    
    AActor* CameraActor = UMyCameraLibrary::GetCameraByName(WorldContextObject, actorName);
    APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

    if (CameraActor && PC)
    {
        PC->SetViewTarget(CameraActor);
        PC->SetIgnoreLookInput(!inputEnabled);
        PC->SetIgnoreMoveInput(!inputEnabled);

        UMyCameraLibrary::ResetAllLights(WorldContextObject);
        
        USpotLightComponent* SpotLight = CameraActor->FindComponentByClass<USpotLightComponent>();
        if (SpotLight)
        {
            SpotLight->SetVisibility(true);
        }
       
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
