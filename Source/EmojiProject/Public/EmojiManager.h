// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmojiActor.h"
#include "GameFramework/Actor.h"
#include "EmojiManager.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Emoji")
	TArray<TSubclassOf<AEmojiActor>> EmojiTypes;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector LeftSpawnPoint;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector RightSpawnPoint;
	
	UPROPERTY(EditDefaultsOnly, Category = "Emoji")
	TSubclassOf<class AEmojiActor> EmojiActorClass;
};


