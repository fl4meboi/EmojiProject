// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiManager.h"

// Sets default values
AEmojiManager::AEmojiManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEmojiManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEmojiManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// void AEmojiManager::SpawnEmojiAtLocation(const FVector& SpawnLocation)
// {
// 	if (EmojiActorClass)
// 	{
// 		AEmojiActor* Emoji = GetWorld()->SpawnActor<AEmojiActor>(EmojiActorClass, SpawnLocation, FRotator::ZeroRotator);
// 		if (Emoji)
// 		{
// 			Emoji->StartPosition = SpawnLocation;
// 			// Set other initial properties here
//
// 		}
// 	}
// }

void AEmojiManager::SpawnEmoji()
{
	if (EmojiTypes.Num() > 0)
	{
		// Randomly select an emoji type
		int32 Index = FMath::RandRange(0, EmojiTypes.Num() - 1);
		// FVector SpawnLocation = (FMath::RandBool() ? LeftSpawnPoint : RightSpawnPoint);
		FVector SpawnLocation = RightSpawnPoint;

		// Spawn emoji
		GetWorld()->SpawnActor<AEmojiActor>(EmojiTypes[Index], SpawnLocation, FRotator::ZeroRotator);
	}
}
