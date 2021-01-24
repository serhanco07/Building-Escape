// Copyright Serhan Corbaci 2021
#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetUpInputComponent();
}
void UGrabber::SetUpInputComponent()
{
	// Input Component Check

	InputComp = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComp)
	{
		/*UInputComponent is found!*/
		InputComp->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComp->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		 UE_LOG(LogTemp, Error, TEXT("%s doesn't have the UInputComponent!!!"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		/*UPhysicsHandleComponent is found!*/
	}
	else
	{
		 UE_LOG(LogTemp, Error, TEXT("%s doesn't have the UPhysicsHandleComponent!!!"), *GetOwner()->GetName());
	}
}
// Called when the game starts

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("Grabber Pressed"));

	GetLineTraceEnd();
	FVector LineTraceEnd = GetLineTraceEnd();
	// Try and reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();
	// If we hit something then attach the physics handle
	if (ActorHit)
	{
		if(!PhysicsHandle){return;}
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, LineTraceEnd);
	}

}
void UGrabber::Release()
{
	if(!PhysicsHandle){return;}
	UE_LOG(LogTemp, Error, TEXT("Grabber Released"));
	PhysicsHandle->ReleaseComponent();
}
// TODO remove/release the physics handle
// Called severy frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	GetLineTraceEnd();
	FVector LineTraceEnd = GetLineTraceEnd();

	// If the physics handle is attached 
	if(!PhysicsHandle){return;}
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	// Move the object we are holding
}
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get Players Viewpoint
	FVector LineTraceEnd = GetLineTraceEnd();
	FVector PlayerViewPointLocation = GetPlayerViewPointLocation();
	// Log out to test
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *(ActorHit->GetName()));
	}
	return Hit;
}
FVector UGrabber::GetLineTraceEnd() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	// Logging out to test

	// UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

	// Ray-cast out to a certain distance

	// Draw a line for player showing the reach
	FVector RotationAsVector = PlayerViewPointRotation.Vector();
	FVector LineTraceDirection = RotationAsVector * Reach;
	FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection;

	return LineTraceEnd;
}
FVector UGrabber::GetPlayerViewPointLocation() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	// Logging out to test

	// UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

	// Ray-cast out to a certain distance

	// Draw a line for player showing the reach
	FVector RotationAsVector = PlayerViewPointRotation.Vector();
	FVector LineTraceDirection = RotationAsVector * Reach;
	FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection;

	return PlayerViewPointLocation;
}