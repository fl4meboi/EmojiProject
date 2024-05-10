// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiPlayerController.h"


void AEmojiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("IE_Spawn", IE_Pressed, this, &AEmojiPlayerController::SpawnEmoji);
	
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
	FString EmojiName = EmojiManager->GetRandomEmojiName();
	if (!EmojiName.IsEmpty())
	{
		EmojiManager->SetCurrentEmoji(EmojiName);
		EmojiManager = GetWorld()->SpawnActor<AEmojiManager>();
	}
}



