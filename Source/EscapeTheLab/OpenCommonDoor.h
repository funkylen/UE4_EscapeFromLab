// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenCommonDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

private:
	FVector DoorLocationOrigin;
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(150.f);

	UInputComponent *InputComponent = nullptr;

	bool isOpened = false;
	bool isOpening = false;
	bool isClosing = false;
	float InitialYaw;
	float CurrentYaw;
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	bool IsPawnBesidesTheDoor() const;	
	void RotateDoor(float &RotateYaw, float &DeltaTime);
	void SwingDoor();
	void OpenDoor(float &DeltaTime);
	void CloseDoor(float &DeltaTime);
};
