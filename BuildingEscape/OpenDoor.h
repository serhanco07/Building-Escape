// Copyright Serhan Corbaci 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void PressurePlateCheck() const;
private:

	float InitialYaw;
	float CurrentYaw;
	float DoorLastOpened = 0.f;
	bool SoundPlayedHowManyTimesOpening = false;
	bool SoundPlayedHowManyTimesClosing = true;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 50.f;
	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.f;
	
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 0.9f;
	
	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 0.405f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

};
