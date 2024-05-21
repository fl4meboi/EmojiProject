// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiPlayerController.h"

#include "EngineUtils.h"

void AEmojiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("IE_Spawn", IE_Pressed, this, &AEmojiPlayerController::SpawnEmoji);
	InputComponent->BindAction("IE_CenterSpawn", IE_Pressed, this, &AEmojiPlayerController::CenterSpawnEmoji);
	
}

void AEmojiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TActorIterator<AEmojiManager> It(GetWorld());		// World에서 EmojiManager 찾는 코드
	EmojiManager = *It;									// EmojiManager의 실체를 넣어주기 
	
	
}

// void AEmojiPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
// 	if (EnhancedInputComponent)
// 	{
// 		EnhancedInputComponent->BindAction(IE_Spawn, ETriggerEvent::Triggered, this, &AEmojiPlayerController::SpawnEmoji);
// 	}
// }

void AEmojiPlayerController::SpawnEmoji()
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnEmoji"));
	FString EmojiName = EmojiManager->GetRandomEmojiName();
	if (!EmojiName.IsEmpty())
	{
		// EmojiManager->SetCurrentEmoji(EmojiName);	// SetCurrentEmoji 함수도 필요 없음 
		// EmojiManager->SpawnEmoji(EmojiName);
		
	}
}

void AEmojiPlayerController::CenterSpawnEmoji()
{
	UE_LOG(LogTemp, Warning, TEXT("CenterSpawnEmoji"));
	FString EmojiName = EmojiManager->GetRandomEmojiName();
	if (!EmojiName.IsEmpty())
	{
		// EmojiManager->CenterSpawnEmoji(EmojiName);
	}
}



