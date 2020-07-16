// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorOpener.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenCommonDoor.h"

#define OUT

// Sets default values for this component's properties
UDoorOpener::UDoorOpener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UDoorOpener::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UDoorOpener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AActor* UDoorOpener::GetDoorBesidesPlayer() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	FHitResult Hit;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation);

	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * 100.f;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
		TraceParams);

	return Hit.GetActor();
}
