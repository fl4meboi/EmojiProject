// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiActor.h"

#include "EmojiPlayerController.h"
#include "EmojiPlayerController.h"
#include "Components/BillboardComponent.h"
#include "PaperFlipbook.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEmojiActor::AEmojiActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetupAttachment(RootComponent);

	CurrentScale = FVector(0.1f, 0.1f, 0.1f);
	TargetScale = FVector(2.f, 2.f, 2.f);
	ScaleSpeed = 2.f;

	// Initial Scale
	SetActorScale3D(CurrentScale);
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
	
	// Move up
	FVector Location = GetActorLocation();
	Location.Z += FloatSpeed * DeltaTime;

	if (MovementType == EEmojiMovementType::Sway)
	{
		Location.Y += FMath::Sin(GetGameTimeSinceCreation() * SwaySpeed) * SwayAmount;
	}
	
	SetActorLocation(Location);

	// Scale up
	if (CurrentScale != TargetScale)
	{
		CurrentScale = FMath::VInterpTo(CurrentScale, TargetScale, DeltaTime, ScaleSpeed);
		SetActorScale3D(CurrentScale);

		UE_LOG(LogTemp, Warning, TEXT("EmojiActor: Emoji scale up"));
	}


}

void AEmojiActor::SetEmojiSpeed()
{
	// Default speed for side-spawning emojis
	FloatSpeed = 250.f;

	// Check movement type to adjust speed
	if (MovementType == EEmojiMovementType::StraightUp)
	{
		FloatSpeed = 90.f;
	}
}

void AEmojiActor::SetNewEmojiSpeed(float NewFloatSpeed, float NewSwayAmount, float NewSwaySpeed)
{
	if (NewFloatSpeed != -1) FloatSpeed = NewFloatSpeed;
	if (NewSwayAmount != -1) SwayAmount = NewSwayAmount;
	if (NewSwaySpeed != -1) SwaySpeed = NewSwayAmount;
}

void AEmojiActor::Init(int32 NewVariationIndex)
{
	VariationIndex = NewVariationIndex;
}

void AEmojiActor::SetEmojiArrayIndex(const TArray<AEmojiActor*>& EmojiArray)
{
	EmojiArrayIndex = EmojiArray.Find(this);
	// EmojiActorArrayPtr = &EmojiArray;
}

int32 AEmojiActor::GetEmojiArrayIndex() const
{
	return EmojiArrayIndex;
}

void AEmojiActor::SaveData(FEmojiData& EmojiData)
{
	EmojiData.VariationIndex = VariationIndex;
}


void AEmojiActor::SetMovementType(EEmojiMovementType NewType)
{
	MovementType = NewType;

	// Set speed based on movement type
	SetEmojiSpeed();
}

void AEmojiActor::DestroyEvent()
{
	OnEmojiDestroyed.ExecuteIfBound(this);
}

