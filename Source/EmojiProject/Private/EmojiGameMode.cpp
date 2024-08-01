// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiGameMode.h"
#include "EmojiProject/Public/EmojiGameMode.h"
#include "Components/InputComponent.h"
#include "GameFramework/Pawn.h"
#include "EmojiGameInstance.h"
#include "EngineUtils.h"
#include "EmojiProject/EmojiProject.h"

#include "EmojiProject/Public/EmojiManager.h"
#include "GameFramework/PlayerStart.h"

void AEmojiGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GameInstance = GetGameInstance<UEmojiGameInstance>();
	check(GameInstance->IsValidLowLevelFast());

	TActorIterator<APlayerStart> It(GetWorld());
	if (*It != nullptr)
	{
		for (;It;++It)
		{
			int32 PlayerStartIndex = FCString::Atoi(*(*It)->PlayerStartTag.ToString());
			if (PlayerStartIndex >= PawnTypeArray.Num())
			{
				GameInstance->LogToFile(LOGTEXT(TEXT("%s is bigger than pawntype array length"), *(*It)->GetActorNameOrLabel()));
				continue;
			}

			PawnTypeArray[PlayerStartIndex].PlayerStart = (*It);
		}
	}

	if (PawnTypeArray.Num() <= 0) return;

	const FString* pPawnType = GameInstance->GetConfigMap().Find(TEXT("PawnType"));
	if (pPawnType == nullptr)
	{
		GameInstance->LogToFile(LOGTEXT(TEXT("Config has no PawnType field")));
		return;
	}

	PawnTypeIndex = FCString::Atoi(** pPawnType);
	if (PawnTypeIndex >= PawnTypeArray.Num())
	{
		GameInstance->LogToFile(LOGTEXT(TEXT("PawnTypeIndex is invalid")));
		return;
	}

	DefaultPawnClass = PawnTypeArray[PawnTypeIndex].PawnClass;

	GameInstance->LogToFile(LOGTEXT(TEXT("PawnClass Name : %s"), *DefaultPawnClass->GetName()));

	TActorIterator<AEmojiManager> EmojiManagerIt(GetWorld());
	if (*EmojiManagerIt == nullptr)
	{
		GameInstance->LogToFile(LOGTEXT(TEXT("Level has no EmojiManager")));
		return;
	}

	(*EmojiManagerIt)->SetSpawnPoint(PawnTypeArray[PawnTypeIndex].LeftSpawnPoint, PawnTypeArray[PawnTypeIndex].RightSpawnPoint, PawnTypeArray[PawnTypeIndex].CenterSpawnPoint);
}

void AEmojiGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

AActor* AEmojiGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (PawnTypeArray.Num() > 0 && PawnTypeIndex != -1)
	{
		if (PawnTypeArray[PawnTypeIndex].PlayerStart->IsValidLowLevelFast())
		{
			GameInstance->LogToFile(LOGTEXT(TEXT("Player Start Location: %s"), *PawnTypeArray[PawnTypeIndex].PlayerStart->GetActorLocation().ToString()));
		}
		else
		{
			GameInstance->LogToFile(LOGTEXT(TEXT("Player Start Location is invalid")));
		}
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

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