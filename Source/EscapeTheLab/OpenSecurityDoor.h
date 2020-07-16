// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"
#include "OpenSecurityDoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPETHELAB_API UOpenSecurityDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenSecurityDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bOpenSound = false;
	bool bCloseSound = true;
	float InitialY;
	float CurrentY;

	UAudioComponent* OpenSoundComponent = nullptr;
	UAudioComponent* CloseSoundComponent = nullptr;

	UPROPERTY(EditAnywhere)
		float OffsetY = 0.f;

	UPROPERTY(EditAnywhere)
		float MassToOpenDoor = 50.f;

	UPROPERTY(EditAnywhere)
		float OpenSpeed = 1.f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	void MoveDoor(float& DeltaTime, float& ToYaw);
	bool FindAudioComponents();
	bool FindPressurePlate() const;
	float GetTotalMassOfActorsOnPlate() const;
	void OpenDoor(float& DeltaTime);
	void CloseDoor(float& DeltaTime);
};
