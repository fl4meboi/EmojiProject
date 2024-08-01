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
	FVector2D LeftSpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D RightSpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D CenterSpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* PlayerStart;
};
/**
 * 
 */
UCLASS()
class EMOJIPROJECT_API AEmojiGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual  AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UEmojiGameInstance* GameInstance;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	int32 PawnTypeIndex = -1;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	TArray<FPawnType> PawnTypeArray;

public:
	// void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	//
	// UFUNCTION()
	// void HandleEmojiSpawn();
	
};


