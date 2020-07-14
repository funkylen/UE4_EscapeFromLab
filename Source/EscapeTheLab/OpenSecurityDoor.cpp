// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenSecurityDoor.h"
#include "Components/AudioComponent.h"

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
	FindAudioComponents();
}

// Called every frame
void UOpenSecurityDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() >= MassToOpenDoor)
	{
		MoveDoor(DeltaTime, OffsetY);

		if (!bOpenSound && OpenSoundComponent)
		{
			OpenSoundComponent->Play();
			bOpenSound = true;
			bCloseSound = false;
		}
	}
	else
	{
		MoveDoor(DeltaTime, InitialY);

		if (!bCloseSound && CloseSoundComponent)
		{
			CloseSoundComponent->Play();
			bCloseSound = true;
			bOpenSound = false;
		}
	}
}

void UOpenSecurityDoor::FindPressurePlate() const
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component on it, but no PressurePlate set."), *GetOwner()->GetName());
	}
}

void UOpenSecurityDoor::MoveDoor(float &DeltaTime, float &ToYaw)
{
	CurrentY = FMath::Lerp(CurrentY, ToYaw, DeltaTime * OpenSpeed);
	FVector DoorLocation = GetOwner()->GetActorLocation();
	DoorLocation.Y = CurrentY;
	GetOwner()->SetActorLocation(DoorLocation);
}

float UOpenSecurityDoor::GetTotalMassOfActorsOnPlate() const
{
	float TotalMass = 0.f;

	if (!PressurePlate)
	{
		return TotalMass;
	}

	TArray<AActor *> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (AActor *Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

bool UOpenSecurityDoor::FindAudioComponents()
{
	TArray<UAudioComponent *> Components;
	GetOwner()->GetComponents<UAudioComponent>(Components);

	if (Components.Num() != 0)
	{
		for (UAudioComponent *AudioComponent : Components)
		{
			if (AudioComponent->GetName() == "OpenSound")
			{
				OpenSoundComponent = AudioComponent;
			}
			else if (AudioComponent->GetName() == "CloseSound")
			{
				CloseSoundComponent = AudioComponent;
			}
		}
		return true;
	}

	return false;
}
