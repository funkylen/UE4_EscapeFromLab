// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Engine/TriggerVolume.h"
#include "PressButton.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPETHELAB_API UPressButton : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPressButton();
	bool IsPressed = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	void Press();
	bool FindInputComponent();
	bool FindPlayersActor();
	bool FindButtonTriggerVolume() const;

	UInputComponent *InputComponent = nullptr;
	AActor *PlayersActor = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume *ButtonTriggerVolume = nullptr;
};
