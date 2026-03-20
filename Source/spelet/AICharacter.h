#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class UAnimSequence;

USTRUCT(BlueprintType)
struct FWaypointData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waypoints")
	AActor* Waypoint = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Waypoints")
	UAnimSequence* AnimationToPlay = nullptr;
};

UCLASS()
class SPELET_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	bool hasStarted = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta = (ClampMin = "1", ClampMax = "20", UIMin = "1", UIMax = "20"))
	int32 AILevel = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	FString name = "Placeholder";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Custom Navigation")
	TArray<FWaypointData> finalWaypoint; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Custom Navigation")
	TArray<FWaypointData> MovementWaypoints;
	
	UFUNCTION(BlueprintCallable, Category="Actions")
	void AttemptMove();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float refreshTime = 10.0f;
	
	UFUNCTION(BlueprintCallable)
	void StartAI();

	// Door check
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Doors")
	TArray<AActor*> DoorActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Doors")
	float DoorCloseGraceTime = 5.0f;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorNotClosed);

	UPROPERTY(BlueprintAssignable, Category="Doors")
	FOnDoorNotClosed OnDoorNotClosed;

	UFUNCTION()
	void OnDoorStateChangedCallback();

	bool alreadyOnFinalWaypoint = false;
	int32 currentWaypoint = 0;
	FTimerHandle MovementTimerHandle;
	FTimerHandle DoorCheckTimerHandle;

private:
	void CheckNearbyDoors();
	void OnGraceTimerExpired();
	void BindDoorDispatchers();
};