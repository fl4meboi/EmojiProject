// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiPlayerController.h"

void AEmojiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("IE_Spawn", IE_Pressed, this, &AEmojiPlayerController::SpawnEmoji);
}

void AEmojiPlayerController::SpawnEmoji()
{
	if (!EmojiManager)
	{
		EmojiManager = GetWorld()->SpawnActor<AEmojiManager>();
	}
	EmojiManager->SpawnEmoji();
}

