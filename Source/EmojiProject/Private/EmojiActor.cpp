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

	CurrentScale = FVector(0.1f, 0.1f, 0.1f);
	TargetScale = FVector(1.0f, 1.0f, 1.0f);
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
	}

	if (Location.Z >= TargetHeight)
	{
		Destroy();
	}
}

void AEmojiActor::SetEmojiArrayIndex(const TArray<AEmojiActor*>& EmojiArray)
{
	EmojiArrayIndex = EmojiArray.Find(this);
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
}

// Mqtt로 받은 사용자가 정한 Emoji에 대한 정보를 받아서 특정 Emoji를 띄워주는 함수 

