// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiGameMode.h"
#include "EmojiProject/Public/EmojiGameMode.h"
#include "Components/InputComponent.h"
#include "GameFramework/Pawn.h"

#include "EmojiProject/Public/EmojiManager.h"

void AEmojiGameMode::BeginPlay()
{
	Super::BeginPlay();
}

// void AEmojiGameMode::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
// 	PlayerInputComponent->BindAction("SpawnEmoji", IE_Pressed, this, &AEmojiGameMode::HandleEmojiSpawn);
// }
//
// void AEmojiGameMode::HandleEmojiSpawn()
// {
// 	// Assuming EmojiManager is set up elsewhere in GameMode or singleton
// 	EmojiManager->SpawnEmojiAtLocation(/* Determine SpawnLocation based on input*/);
// }