// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiManager.h"

#include "CenterEmojiActor.h"
#include "MqttManager.h"
#include "EmojiGameInstance.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
// #include "UE4Library/NotificationType.Pregenerated.h"

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

	// if (bIsLoading || bIsSpawning) return;

	// if (MqttManager && MqttManager->HasMessage())
	// {
	// 	// bIsSpawning = true;
	//
	// 	// MqttManager->RequestEmojiData();
	// 	// Spawn 코드를 써야 함!
	// 	SpawnEmoji();
	// }
}

// void AEmojiManager::SetCurrentEmoji(const FString& EmojiName)
// {
// 	CurrentEmojiName = EmojiName;
// }

FString AEmojiManager::GetRandomEmojiName()
{
	if (EmojiMap.Num() > 0)
	{
		// Get all keys from the map
		TArray<FString> Keys;
		EmojiMap.GetKeys(Keys);

		// Select a random key
		int32 RandomIndex = FMath::RandRange(0, Keys.Num() - 1);
		return Keys[RandomIndex];
	}

	return FString();	// Return an empty string if the map is empty
}

void AEmojiManager::SpawnEmoji(FString EmojiName)
{
	UWorld* World = GetWorld();
	check(World);
		
	if (EmojiMap.Num() > 0)
	{
		const TSubclassOf<AEmojiActor>* FoundEmojiClass = EmojiMap.Find(EmojiName);
		if (FoundEmojiClass)
		{
			FVector SpawnLocation = (FMath::RandBool() ? LeftSpawnPoint : RightSpawnPoint);

			AEmojiActor* EmojiActor = GetWorld()->SpawnActor<AEmojiActor>(*FoundEmojiClass, SpawnLocation, FRotator::ZeroRotator);
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
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Emoji with name %s not found."), *EmojiName);
		}
	}

	// bIsSpawning = false;
}

void AEmojiManager::CenterSpawnEmoji(FString EmojiName)
{
	UWorld* World = GetWorld();
	check(World);

	if (EmojiMap.Num() > 0)
	{
		const TSubclassOf<AEmojiActor>* FoundEmojiClass = EmojiMap.Find(EmojiName);
		if (FoundEmojiClass)
		{
			FVector SpawnLocation = CenterSpawnPoint;

			// Spawn emoji
			AEmojiActor* EmojiActor = GetWorld()->SpawnActor<AEmojiActor>(*FoundEmojiClass, SpawnLocation, FRotator::ZeroRotator);
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
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Emoji with name %s not found."), *EmojiName);
		}
	}
	
	// bIsSpawning = false;
}

void AEmojiManager::SideOrCenter(bool bIsCenter, FString EmojiName)
{
	// MqttManager->ParseMessage(const FString& Message);
	if (bIsCenter)
	{
		CenterSpawnEmoji(EmojiName);
	}
	else
	{
		SpawnEmoji(EmojiName);
	}
}

void AEmojiManager::ShowEmoji(bool bIsShow)
{
	// MqttManager에서 power true/false push 받으면
	// 돌면서 visibility 
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

// void AEmojiManager::LoadEmoji(TArray<FEmojiData>& EmojiDataArray)		// 필요없음 
// {
// 	UWorld* World = GetWorld();
// 	check(World);
//
// 	FActorSpawnParameters Params;
// 	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
// 	for (auto EmojiData : EmojiDataArray)
// 	{
// 		AEmojiActor* Emoji = World->SpawnActor<AEmojiActor>(EmojiClassArray[EmojiData.VariationIndex],
// 			GetActorLocation(), GetActorRotation() + FRotator(0, 180, 0), Params);
//
// 		// Emoji Data 갱신
// 		// Emoji->
//
// 		
// 	}
// }

// void AEmojiManager::ToggleVisibility()
// {
// 	bool bIsVisible = AEmojiActor->
// }
