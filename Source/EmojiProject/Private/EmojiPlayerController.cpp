// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiPlayerController.h"

#include "EngineUtils.h"

void AEmojiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("IE_Spawn", IE_Pressed, this, &AEmojiPlayerController::SpawnEmoji);
	InputComponent->BindAction("IE_CenterSpawn", IE_Pressed, this, &AEmojiPlayerController::CenterSpawnEmoji);
	// InputComponent->BindAction("IE_Hide", IE_Hide, this, &AEmojiPlayerController::Hide);
}

void AEmojiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TActorIterator<AEmojiManager> It(GetWorld());		// World에서 EmojiManager 찾는 코드
	EmojiManager = *It;									// EmojiManager의 실체를 넣어주기 
	
	
}

void AEmojiPlayerController::SpawnEmoji()
{
	UE_LOG(LogTemp, Warning, TEXT("EmojiPlayerController::SpawnEmoji: Emoji side spawned"));
	FString EmojiName = EmojiManager->GetRandomEmojiName();
	if (!EmojiName.IsEmpty())
	{
		// EmojiManager->SetCurrentEmoji(EmojiName);	// SetCurrentEmoji 함수도 필요 없음
		bool bIsCenter = false;
		EmojiManager->SpawnEmoji(EmojiName, bIsCenter);
		UE_LOG(LogTemp, Warning, TEXT("EmojiPlayerController::SpawnEmoji: %s spawned"), *EmojiName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EmojiPlayerController::SpawnEmoji: Emoji %s not found"), *EmojiName);
	}
}

void AEmojiPlayerController::CenterSpawnEmoji()
{
	UE_LOG(LogTemp, Warning, TEXT("EmojiPlayerController::SpawnEmoji: Emoji center spawned"));
	FString EmojiName = EmojiManager->GetRandomEmojiName();
	if (!EmojiName.IsEmpty())
	{
		bool bIsCenter = true;
		EmojiManager->CenterSpawnEmoji(EmojiName, bIsCenter);
		UE_LOG(LogTemp, Warning, TEXT("EmojiPlayerController::SpawnEmoji: %s spawned"), *EmojiName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EmojiPlayerController::SpawnEmoji: Emoji %s not found"), *EmojiName);
	}
}

// void AEmojiPlayerController::Hide()
// {
// 	UE_LOG(LogTemp, Warning, TEXT("EmojiPlayerController::Hide"));
//
// 	EmojiManager->
// }



