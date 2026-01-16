// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSitCharacter.h"

#include "AITestsCommon.h"
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

// Called when the game starts or when spawned
void APlayerSitCharacter::BeginPlay()
{
	Super::BeginPlay();
	PC = Cast<APlayerController>(GetController());
	
	PC->bShowMouseCursor = true;
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;
	
	PC->PlayerCameraManager->ViewPitchMin = -30.0f;
	PC->PlayerCameraManager->ViewPitchMax = 45.0f;
	
	PC->PlayerCameraManager->ViewYawMin = -60.f;
	PC->PlayerCameraManager->ViewYawMax = 60.0f;
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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Did not find PC in Tick Function in player sit."));
	}
}

void APlayerSitCharacter::Interact()
{
	
	FVector Start = PC->GetMousePosition(); //FollowCamera->GetComponentLocation();
	FVector End = Start + (PC->GetControlRotation().Vector() * InteractDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f, 0, 1.f);
#endif
	
	if (!bHit) return;

	AActor* HitActor = Hit.GetActor();
	if (!HitActor) return;

	if (HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable::Execute_Interact(HitActor);
	}
}

void APlayerSitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerSitCharacter::Interact);
}

