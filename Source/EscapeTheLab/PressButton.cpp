// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenCommonDoor.h"
#include "PressButton.h"

// Sets default values for this component's properties
UPressButton::UPressButton()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPressButton::BeginPlay()
{
	Super::BeginPlay();

	FindAudioComponent();

	if (FindDoorToOpen() && FindButtonTriggerVolume() && FindInputComponent() && FindPlayersActor())
	{
		InputComponent->BindAction("Use", IE_Pressed, this, &UPressButton::Press);
	}
}

// Called every frame
void UPressButton::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPressButton::Press()
{
	if (IsPressed)
	{
		return;
	}

	if (!ButtonTriggerVolume->IsOverlappingActor(PlayersActor))
	{
		return;
	}

	UOpenCommonDoor *OpenCommonDoorComponent = DoorToOpen->FindComponentByClass<UOpenCommonDoor>();

	if (!OpenCommonDoorComponent)
	{
		return;
	}

	IsPressed = true;
	FVector ButtonLocation = GetOwner()->GetActorLocation();
	ButtonLocation.Z -= 8;
	if (PressSound)
	{
		PressSound->Play();
	}
	GetOwner()->SetActorLocation(ButtonLocation);
	OpenCommonDoorComponent->IsAllowedToOpen = true;
}

bool UPressButton::FindInputComponent()
{
	InputComponent = GetWorld()->GetFirstPlayerController()->FindComponentByClass<UInputComponent>();

	if (!InputComponent)
	{
		return false;
	}

	return true;
}

bool UPressButton::FindButtonTriggerVolume() const
{
	if (!ButtonTriggerVolume)
	{
		return false;
	}

	return true;
}

bool UPressButton::FindPlayersActor()
{
	PlayersActor = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PlayersActor)
	{
		return false;
	}

	return true;
}

bool UPressButton::FindDoorToOpen() const
{
	if (!DoorToOpen)
	{
		return false;
	}

	return true;
}

bool UPressButton::FindAudioComponent()
{
	PressSound = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!PressSound)
	{
		return false;
	}

	return true;
}