// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Actor.h"
#include "OpenCommonDoor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "PressButton.h"
#include "Components/AudioComponent.h"
#include "DoorOpener.h"

#define OUT

// Sets default values for this component's properties
UOpenCommonDoor::UOpenCommonDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenCommonDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	SetupDoorLocationOrigin();
	FindAudioComponents();

	if (FindPlayersActor() && FindInputComponent() && FindCommonDoorTriggerVolume())
	{
		InputComponent->BindAction("Use", IE_Pressed, this, &UOpenCommonDoor::SwingDoor);
	}
}

// Called every frame
void UOpenCommonDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CommonDoorUnlockedSoundComponent && IsAllowedToOpen && !UnlockDoorSound)
	{
		CommonDoorUnlockedSoundComponent->Play();
		UnlockDoorSound = true;
	}

	if (IsOpening)
	{
		OpenDoor(DeltaTime);
	}
	else if (IsClosing)
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenCommonDoor::OpenDoor(float &DeltaTime)
{
	RotateDoor(OpenAngle, DeltaTime);

	if (!CommonDoorOpenSoundComponent)
	{
		return;
	}

	if (!OpenDoorSound)
	{
		CommonDoorOpenSoundComponent->Play();
		OpenDoorSound = true;
		CloseDoorSound = false;
	}
}

void UOpenCommonDoor::CloseDoor(float &DeltaTime)
{
	RotateDoor(InitialYaw, DeltaTime);

	if (!CommonDoorCloseSoundComponent)
	{
		return;
	}

	if (!CloseDoorSound)
	{
		CommonDoorCloseSoundComponent->Play();
		CloseDoorSound = true;
		OpenDoorSound = false;
	}
}

bool UOpenCommonDoor::IsPawnBesidesTheDoor() const
{
	UDoorOpener *DoorOpener = PlayersActor->FindComponentByClass<UDoorOpener>();

	if (!DoorOpener)
	{
		return false;
	}

	AActor *Door = DoorOpener->GetDoorBesidesPlayer();

	if (!Door)
	{
		return false;
	}

	return Door->GetName() == GetOwner()->GetName() && CommonDoorTriggerVolume->IsOverlappingActor(PlayersActor);
}

void UOpenCommonDoor::RotateDoor(float &RotateYaw, float &DeltaTime)
{
	if (round(CurrentYaw) == RotateYaw)
	{
		IsOpened = RotateYaw == OpenAngle;
		IsOpening = false;
		IsClosing = false;
		return;
	}

	CurrentYaw = FMath::Lerp(CurrentYaw, RotateYaw, DeltaTime * 5.f);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenCommonDoor::SwingDoor()
{
	if (!IsPawnBesidesTheDoor())
	{
		return;
	}

	if (!IsAllowedToOpen && CommonDoorLockedSoundComponent)
	{
		CommonDoorLockedSoundComponent->Play();
	}

	if (IsOpened)
	{
		IsOpening = false;
		IsClosing = true;
	}
	else
	{
		IsOpening = true;
		IsClosing = false;
	}
}

bool UOpenCommonDoor::FindPlayersActor()
{
	PlayersActor = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PlayersActor)
	{
		return false;
	}

	return true;
}

bool UOpenCommonDoor::FindInputComponent()
{
	InputComponent = GetWorld()->GetFirstPlayerController()->FindComponentByClass<UInputComponent>();

	if (!InputComponent)
	{
		return false;
	}

	return true;
}

bool UOpenCommonDoor::FindCommonDoorTriggerVolume() const
{
	if (!CommonDoorTriggerVolume)
	{
		return false;
	}

	return true;
}

bool UOpenCommonDoor::FindAudioComponents()
{
	TArray<UAudioComponent *> Components;
	GetOwner()->GetComponents<UAudioComponent>(Components);

	if (Components.Num() != 0)
	{
		for (UAudioComponent *AudioComponent : Components)
		{
			if (AudioComponent->GetName() == "CommonDoorOpenSound")
			{
				CommonDoorOpenSoundComponent = AudioComponent;
			}
			else if (AudioComponent->GetName() == "CommonDoorCloseSound")
			{
				CommonDoorCloseSoundComponent = AudioComponent;
			}
			else if (AudioComponent->GetName() == "CommonDoorLockedSound")
			{
				CommonDoorLockedSoundComponent = AudioComponent;
			}
			else if (AudioComponent->GetName() == "CommonDoorUnlockedSound")
			{
				CommonDoorUnlockedSoundComponent = AudioComponent;
			}
		}
		return true;
	}

	return false;
}

void UOpenCommonDoor::SetupDoorLocationOrigin()
{
	FVector BoxExtent;
	GetOwner()->GetActorBounds(false, DoorLocationOrigin, BoxExtent);
}