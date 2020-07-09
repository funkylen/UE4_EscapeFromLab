// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenSecurityDoor.h"

// Sets default values for this component's properties
UOpenSecurityDoor::UOpenSecurityDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenSecurityDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialY = GetOwner()->GetActorLocation().Y;
	CurrentY = InitialY;

	FindPressurePlate();
}

// Called every frame
void UOpenSecurityDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OpenDoor(DeltaTime);
}

void UOpenSecurityDoor::FindPressurePlate() const
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component on it, but no PressurePlate set."), *GetOwner()->GetName());
	}
}

void UOpenSecurityDoor::OpenDoor(float &DeltaTime)
{
	CurrentY = FMath::Lerp(CurrentY, OffsetY, DeltaTime * OpenSpeed);
	FVector DoorLocation = GetOwner()->GetActorLocation();
	DoorLocation.Y = CurrentY;
	GetOwner()->SetActorLocation(DoorLocation);
}
