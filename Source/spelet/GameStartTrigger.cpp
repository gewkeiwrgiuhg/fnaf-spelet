// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartTrigger.h"
#include "WB_Objective.h"
#include "PlayerCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameStartTrigger::AGameStartTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OnActorBeginOverlap.AddDynamic(this, &AGameStartTrigger::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGameStartTrigger::BeginPlay()
{
	Super::BeginPlay();
	Wall1->SetActorEnableCollision(false);
	Wall2->SetActorEnableCollision(false);
}

// Called every frame
void AGameStartTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameStartTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		if (AIToActivate)
		{
			AIToActivate->StartAI();
		}
		
		if (Wall1)
			Wall1->SetActorEnableCollision(true);

		if (Wall2)
			Wall2->SetActorEnableCollision(true);
		
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			TArray<UUserWidget*> FoundWidgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UWB_Objective::StaticClass(), false);

			if (FoundWidgets.Num() > 0)
			{
				ObjectiveWidget = Cast<UWB_Objective>(FoundWidgets[0]);
			}

			if (ObjectiveWidget)
			{
				ObjectiveWidget->SetObjective(FText::FromString(TEXT("Survive until 6 AM")));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Objective Widget not found"));
			}
		}
	}
}