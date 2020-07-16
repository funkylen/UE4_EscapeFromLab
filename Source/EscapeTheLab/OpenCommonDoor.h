// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"
#include "OpenCommonDoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPETHELAB_API UOpenCommonDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenCommonDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
		bool bIsAllowedToOpen = false;

private:
	bool bIsOpened = false;
	bool bIsOpening = false;
	bool bIsClosing = false;
	bool bOpenDoorSound = false;
	bool bCloseDoorSound = false;
	bool bUnlockDoorSound = false;
	float InitialYaw;
	float CurrentYaw;

	FVector DoorLocationOrigin;

	AActor* PlayersActor = nullptr;
	UInputComponent* InputComponent = nullptr;
	UAudioComponent* CommonDoorOpenSoundComponent = nullptr;
	UAudioComponent* CommonDoorCloseSoundComponent = nullptr;
	UAudioComponent* CommonDoorLockedSoundComponent = nullptr;
	UAudioComponent* CommonDoorUnlockedSoundComponent = nullptr;

	UPROPERTY(EditAnywhere)
		float OpenAngle = 90.f;
	UPROPERTY(EditAnywhere)
		ATriggerVolume* CommonDoorTriggerVolume = nullptr;

	bool IsPawnBesidesTheDoor() const;
	bool FindCommonDoorTriggerVolume() const;
	bool FindPlayersActor();
	bool FindInputComponent();
	bool FindAudioComponents();
	void RotateDoor(float& RotateYaw, float& DeltaTime);
	void SwingDoor();
	void OpenDoor(float& DeltaTime);
	void CloseDoor(float& DeltaTime);
	void SetupDoorLocationOrigin();
};
