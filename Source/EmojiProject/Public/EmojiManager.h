// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmojiActor.h"
#include "GameFramework/Actor.h"
#include "EmojiManager.generated.h"

class AMqttManager;
class AEmojiActor;

struct FEmojiData;

USTRUCT()
struct FEmojiType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AEmojiActor>> EmojiClass;
};

UCLASS()
class EMOJIPROJECT_API AEmojiManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmojiManager();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Emoji")
	// void SpawnEmojiAtLocation(const FVector& SpawnLocation);
	void SpawnEmoji();

	UFUNCTION(BlueprintCallable, Category = "Emoji")
	void CenterSpawnEmoji();
	
	// UFUNCTION(BlueprintCallable, Category = "Emoji")
	// void RemoveAllEmoji();

	const TArray<AEmojiActor*>& GetEmojiArray() const;

	void LoadEmoji(TArray<FEmojiData>& EmojiDataArray);

	// void ToggleVisibility();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// UPROPERTY(EditAnywhere, Category = "Emoji")
	// TArray<TSubclassOf<AEmojiActor>> EmojiTypes;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector LeftSpawnPoint;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector RightSpawnPoint;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector CenterSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Emoji")
	TArray<TSubclassOf<AEmojiActor>> EmojiClassArray;

	UPROPERTY(EditAnywhere, Category = "Emoji")
	TArray<TSubclassOf<AEmojiActor>> CenterEmojiClassArray;
	
	// UPROPERTY(EditDefaultsOnly, Category = "Emoji")
	// TArray<TSubclassOf<class AEmojiActor>> EmojiClassArray;

private:
	UPROPERTY()
	TArray<AEmojiActor*> EmojiArray;

	UPROPERTY()
	TArray<AEmojiActor*> CenterEmojiArray;
};


