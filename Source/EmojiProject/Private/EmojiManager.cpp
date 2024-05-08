// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiManager.h"

#include "CenterEmojiActor.h"

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

	// for (FEmojiType& EmojiType : )
	
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
	if (EmojiClassArray.Num() > 0)
	{
		// Randomly select an emoji type
		int32 Index = FMath::RandRange(0, EmojiClassArray.Num() - 1);
		FVector SpawnLocation = (FMath::RandBool() ? LeftSpawnPoint : RightSpawnPoint);
		// FVector SpawnLocation = RightSpawnPoint;

		// Spawn emoji
		AEmojiActor* EmojiActor = GetWorld()->SpawnActor<AEmojiActor>(EmojiClassArray[Index], SpawnLocation, FRotator::ZeroRotator);
		EmojiArray.Add(EmojiActor);
	}
}

void AEmojiManager::CenterSpawnEmoji()
{
	// if (EmojiClassArray.Num() > 0)
	// {
	// 	// Randomly select an emoji type
	// 	int32 Index = FMath::RandRange(0, CenterEmojiClassArray.Num() - 1);
	// 	FVector SpawnLocation = CenterSpawnPoint;
	//
	// 	// Spawn emoji	
	// 	ACenterEmojiActor* CenterEmojiActor = GetWorld()->SpawnActor<ACenterEmojiActor>(CenterEmojiClassArray[Index], SpawnLocation, FRotator::ZeroRotator);
	// 	CenterEmojiArray.Add(CenterEmojiActor);
	// }

	// Randomly select an emoji type
	int32 Index = FMath::RandRange(0, EmojiClassArray.Num() - 1);
	FVector SpawnLocation = CenterSpawnPoint;

	// Spawn emoji
	AEmojiActor* EmojiActor = GetWorld()->SpawnActor<AEmojiActor>(EmojiClassArray[Index], SpawnLocation, FRotator::ZeroRotator);
	EmojiArray.Add(EmojiActor);
}

// void AEmojiManager::RemoveAllEmoji()
// {
// 	for (int i = 0; i < EmojiArray.Num(); ++i)
// 	{
// 		EmojiArray[i]->Destroy();
// 	}
// 	EmojiArray.Empty();
// }

const TArray<AEmojiActor*>& AEmojiManager::GetEmojiArray() const
{
	return EmojiArray;
}

void AEmojiManager::LoadEmoji(TArray<FEmojiData>& EmojiDataArray)
{
	UWorld* World = GetWorld();
	check(World);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (auto EmojiData : EmojiDataArray)
	{
		AEmojiActor* Emoji = World->SpawnActor<AEmojiActor>(EmojiClassArray[EmojiData.VariationIndex],
			GetActorLocation(), GetActorRotation() + FRotator(0, 180, 0), Params);

		// Emoji Data 갱신
		// Emoji->

		
	}
}

// void AEmojiManager::ToggleVisibility()
// {
// 	bool bIsVisible = AEmojiActor->
// }
