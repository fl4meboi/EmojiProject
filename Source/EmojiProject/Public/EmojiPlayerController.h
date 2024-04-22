// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmojiManager.h"
#include "GameFramework/PlayerController.h"
#include "EmojiPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EMOJIPROJECT_API AEmojiPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

private:
	AEmojiManager* EmojiManager;

	// UPROPERTY(EditAnywhere)
	// class UInputAction* IE_Spawn;

	void SpawnEmoji();
	
};
