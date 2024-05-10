// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiManager.h"

#include "CenterEmojiActor.h"
#include "MqttManager.h"
#include "EmojiGameInstance.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

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

	GameInstance = GetGameInstance<UEmojiGameInstance>();
	check(GameInstance);

	// Set Config Data
	const TMap<FString, FString>& ConfigMap = GameInstance->GetConfigMap();

	FloatSpeed = FCString::Atof(**ConfigMap.Find(FString("floatspeed")));
	SwayAmount = FCString::Atof(**ConfigMap.Find(FString("swayamount")));
	SwaySpeed = FCString::Atof(**ConfigMap.Find(FString("swayspeed")));

	// Get MqttManager
	TActorIterator<AMqttManager> It(GetWorld());
	MqttManager = *It;
	check(MqttManager);
}

// Called every frame
void AEmojiManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLoading || bIsSpawning) return;

	if (MqttManager && MqttManager->HasMessage())
	{
		bIsSpawning = true;

		// MqttManager->RequestEmojiData();
	}
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

void AEmojiManager::SpawnEmoji(const FString& EmojiName)
{
	UWorld* World = GetWorld();
	check(World);
		
	if (EmojiClassArray.Num() > 0)
	{
		// Randomly select an emoji type
		int32 Index = FMath::RandRange(0, EmojiClassArray.Num() - 1);
		FVector SpawnLocation = (FMath::RandBool() ? LeftSpawnPoint : RightSpawnPoint);
		// FVector SpawnLocation = RightSpawnPoint;

		// Spawn emoji
		AEmojiActor* EmojiActor = GetWorld()->SpawnActor<AEmojiActor>(EmojiClassArray[Index], SpawnLocation, FRotator::ZeroRotator);
		if (EmojiActor)
		{
			EmojiActor->SetMovementType(AEmojiActor::EEmojiMovementType::Sway);
			EmojiArray.Add(EmojiActor);
		}

		if (EmojiSpawnSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), EmojiSpawnSound);
		}
	}

	bIsSpawning = false;
}

void AEmojiManager::CenterSpawnEmoji(const FString& EmojiName)
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

	UWorld* World = GetWorld();
	check(World);
	
	if (EmojiClassArray.Num() > 0)
	{
		// Randomly select an emoji type
		int32 Index = FMath::RandRange(0, EmojiClassArray.Num() - 1);
		FVector SpawnLocation = CenterSpawnPoint;

		// Spawn emoji
		AEmojiActor* EmojiActor = GetWorld()->SpawnActor<AEmojiActor>(EmojiClassArray[Index], SpawnLocation, FRotator::ZeroRotator);
		if (EmojiActor)
		{
			EmojiActor->SetMovementType(AEmojiActor::EEmojiMovementType::StraightUp);
			EmojiArray.Add(EmojiActor);
		}

		if (CenterEmojiSpawnSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), CenterEmojiSpawnSound);
		}
	}

	bIsSpawning = false;
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
