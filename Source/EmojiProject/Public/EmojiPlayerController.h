// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmojiManager.h"
#include "GameFramework/PlayerController.h"
// #include "EnhancedInputComponent.h"
#include "EmojiPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EMOJIPROJECT_API AEmojiPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	virtual void SetupInputComponent() override;

	void SpawnEmoji();

private:
	AEmojiManager* EmojiManager;

	// UPROPERTY(EditAnywhere)
	// UInputAction* IE_Spawn;

	FString CurrentEmojiName;  // Stores the current emoji name to spawn	
	
};
