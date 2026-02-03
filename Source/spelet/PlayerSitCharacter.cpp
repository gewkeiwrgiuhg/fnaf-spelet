#include "PlayerSitCharacter.h"
#include "DrawDebugHelpers.h"
#include "Interactable.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerSitCharacter::APlayerSitCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 0.0f;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->bEnableCameraRotationLag = true;
    
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void APlayerSitCharacter::BeginPlay()
{
    Super::BeginPlay();
    PC = Cast<APlayerController>(GetController());
    
    if (PC)
    {
        PC->bShowMouseCursor = true;
        PC->bEnableClickEvents = true;
        PC->bEnableMouseOverEvents = true;

        PC->PlayerCameraManager->ViewPitchMin = pitchMin;
        PC->PlayerCameraManager->ViewPitchMax = pitchMax;
        PC->PlayerCameraManager->ViewYawMin = yawMin;
        PC->PlayerCameraManager->ViewYawMax = yawMax;

        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
        InputMode.SetHideCursorDuringCapture(false);
        PC->SetInputMode(InputMode);
        
        //PC->SetIgnoreLookInput(false);
        //PC->SetIgnoreMoveInput(false);
    }
}

void APlayerSitCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (PC)
    {
       FVector2D MousePosition;
       if (PC->GetMousePosition(MousePosition.X, MousePosition.Y))
       {
          FVector2D ViewportSize;
          GEngine->GameViewport->GetViewportSize(ViewportSize);
          
          float normalizedX = (MousePosition.X / ViewportSize.X) * 2.f - 1.f;
          float normalizedY = (MousePosition.Y / ViewportSize.Y) * 2.f - 1.f;
          
          if (FMath::Abs(normalizedX) < DeadzoneThreshold) normalizedX = 0.f;
          if (FMath::Abs(normalizedY) < DeadzoneThreshold) normalizedY = 0.f;
          
          float TurnValue = (normalizedX > 0.f) ? (normalizedX - DeadzoneThreshold) : (normalizedX + DeadzoneThreshold);
          float LookValue = (normalizedY > 0.f) ? (normalizedY - DeadzoneThreshold) : (normalizedY + DeadzoneThreshold);
          
          AddControllerYawInput(TurnValue * EdgeTurnSpeed * DeltaTime);
          AddControllerPitchInput(LookValue * EdgeTurnSpeed * DeltaTime);
       }
    }
}

void APlayerSitCharacter::Interact()
{
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("No player controller found when interacting."));
        return;
    }

    FVector WorldLocation;
    FVector WorldDirection;

    if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
    {
        FVector Start = WorldLocation;
        FVector End = Start + (WorldDirection * InteractDistance);

        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);

        bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

#if WITH_EDITOR
        DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f, 0, 1.f);
#endif
        
        if (bHit)
        {
            AActor* HitActor = Hit.GetActor();
            if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
            {
                IInteractable::Execute_Interact(HitActor);
            }
        }
    }
}

void APlayerSitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerSitCharacter::Interact);
}