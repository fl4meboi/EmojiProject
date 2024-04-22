// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiActor.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AEmojiActor::AEmojiActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetupAttachment(RootComponent);
	// RootComponent = FlipbookComponent;
}

// Called when the game starts or when spawned
void AEmojiActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEmojiActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location = GetActorLocation();
	Location.Z += FloatSpeed * DeltaTime;
	Location.Y += FMath::Sin(GetGameTimeSinceCreation() * SwaySpeed) * SwayAmount;
	SetActorLocation(Location);

	if (Location.Z >= TargetHeight)
	{
		Destroy();
	}
}

