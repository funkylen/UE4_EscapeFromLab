// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Actor.h"
#include "OpenCommonDoor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "PressButton.h"
#include "DrawDebugHelpers.h"
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

	FRotator DoorRotation = GetOwner()->GetActorRotation();

	FVector BoxExtent;
	GetOwner()->GetActorBounds(false, DoorLocationOrigin, BoxExtent);

	if (FindPlayersActor() && FindInputComponent() && FindCommonDoorTriggerVolume())
	{
		InputComponent->BindAction("Use", IE_Pressed, this, &UOpenCommonDoor::SwingDoor);
	}
}

// Called every frame
void UOpenCommonDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isOpening)
	{
		RotateDoor(OpenAngle, DeltaTime);
	}
	else if (isClosing)
	{
		RotateDoor(InitialYaw, DeltaTime);
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

	UE_LOG(LogTemp, Warning, TEXT("here %s"), *Door->GetName());

	return Door->GetName() == GetOwner()->GetName() && CommonDoorTriggerVolume->IsOverlappingActor(PlayersActor);
}

void UOpenCommonDoor::RotateDoor(float &RotateYaw, float &DeltaTime)
{
	if (round(CurrentYaw) == RotateYaw)
	{
		isOpened = RotateYaw == OpenAngle;
		isOpening = false;
		isClosing = false;
		return;
	}

	CurrentYaw = FMath::Lerp(CurrentYaw, RotateYaw, DeltaTime * 2.f);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenCommonDoor::SwingDoor()
{
	if (!IsPawnBesidesTheDoor())
	{
		// UE_LOG(LogTemp, Error, TEXT("NOT BESIDE THE DOOR!"));
		return;
	}

	if (!IsAllowedToOpen)
	{
		if (!FindAllowButton())
		{
			return;
		}

		if (!AllowButton->FindComponentByClass<UPressButton>()->IsPressed)
		{
			UE_LOG(LogTemp, Warning, TEXT("Open is not allowed!"));
			return;
		}
	}

	if (isOpened)
	{
		isOpening = false;
		isClosing = true;
	}
	else
	{
		isOpening = true;
		isClosing = false;
	}
}

bool UOpenCommonDoor::FindPlayersActor()
{
	PlayersActor = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PlayersActor)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayersActor not found in %s"), *GetOwner()->GetName());
		return false;
	}

	return true;
}

bool UOpenCommonDoor::FindInputComponent()
{
	InputComponent = GetWorld()->GetFirstPlayerController()->FindComponentByClass<UInputComponent>();

	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent not found in %s"), *GetOwner()->GetName());
		return false;
	}

	return true;
}

bool UOpenCommonDoor::FindCommonDoorTriggerVolume() const
{
	if (!CommonDoorTriggerVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("CommonDoorTriggerVolume not found in %s"), *GetOwner()->GetName());
		return false;
	}

	return true;
}

bool UOpenCommonDoor::FindAllowButton() const
{
	if (!AllowButton)
	{
		UE_LOG(LogTemp, Error, TEXT("AllowButton not found in %s"), *GetOwner()->GetName());
		return false;
	}

	return true;
}