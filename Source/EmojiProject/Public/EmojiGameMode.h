// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EmojiGameMode.generated.h"

USTRUCT(BlueprintType)
struct FPawnType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> PawnClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;
};
/**
 * 
 */
UCLASS()
class EMOJIPROJECT_API AEmojiGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	// void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	//
	// UFUNCTION()
	// void HandleEmojiSpawn();
	
};


