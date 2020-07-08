// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Actor.h"
#include "OpenCommonDoor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

	InputComponent = GetWorld()->GetFirstPlayerController()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
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
	TArray<FHitResult> Hits;

	return GetWorld()->SweepMultiByObjectType(
		OUT Hits,
		DoorLocationOrigin,
		DoorLocationOrigin,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
		CollisionSphere);
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