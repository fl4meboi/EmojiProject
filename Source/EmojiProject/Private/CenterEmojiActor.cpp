// Fill out your copyright notice in the Description page of Project Settings.


#include "CenterEmojiActor.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACenterEmojiActor::ACenterEmojiActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetupAttachment(RootComponent);

	CurrentScale = FVector(0.1f, 0.1f, 0.1f);
	TargetScale = FVector(1.0f, 1.0f, 1.0f);
	ScaleSpeed = 2.f;

	// Initial Scale
	SetActorScale3D(CurrentScale);
}

// Called when the game starts or when spawned
void ACenterEmojiActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACenterEmojiActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move up
	FVector Location = GetActorLocation();
	Location.Z += FloatSpeed * DeltaTime;
	SetActorLocation(Location);

	// Scale up
	if (CurrentScale != TargetScale)
	{
		CurrentScale = FMath::VInterpTo(CurrentScale, TargetScale, DeltaTime, ScaleSpeed);
		SetActorScale3D(CurrentScale);
	}

	if (Location.Z >= TargetHeight)
	{
		Destroy();
	}
}

