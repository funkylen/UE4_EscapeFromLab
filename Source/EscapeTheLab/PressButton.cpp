// Fill out your copyright notice in the Description page of Project Settings.

#include "PressButton.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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

	if (FindInputComponent() && FindButtonTriggerVolume() && FindPlayersActor())
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
	if (!ButtonTriggerVolume->IsOverlappingActor(PlayersActor))
	{
		return;
	}

	IsPressed = true;
	FVector ButtonLocation = GetOwner()->GetActorLocation();
	ButtonLocation.Z -= 8;
	GetOwner()->SetActorLocation(ButtonLocation);
}

bool UPressButton::FindInputComponent()
{
	InputComponent = GetWorld()->GetFirstPlayerController()->FindComponentByClass<UInputComponent>();

	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent not found in %s"), *GetOwner()->GetName());
		return false;
	}

	return true;
}

bool UPressButton::FindButtonTriggerVolume() const
{
	if (!ButtonTriggerVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("ButtonTriggerVolume not found in %s"), *GetOwner()->GetName());
		return false;
	}

	return true;
}

bool UPressButton::FindPlayersActor()
{
	PlayersActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	if (!PlayersActor)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayersActor not found in %s"), *GetOwner()->GetName());
		return false;
	}

	return true;
}
