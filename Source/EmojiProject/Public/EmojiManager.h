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
	void SpawnEmoji(FString EmojiName);

	UFUNCTION(BlueprintCallable, Category = "Emoji")
	void CenterSpawnEmoji(FString EmojiName);

	void OnEmojiDestroyed(AEmojiActor* DestroyedEmoji);

	void SideOrCenter(bool bIsCenter, FString EmojiName);

	UFUNCTION(BlueprintCallable, Category = "Emoji")
	void ShowEmoji(bool bShowEmoji);
	
	FString GetRandomEmojiName();
	
	const TArray<AEmojiActor*>& GetEmojiArray() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector LeftSpawnPoint;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector RightSpawnPoint;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector CenterSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Emoji")
	TMap<FString, TSubclassOf<AEmojiActor>> EmojiMap;

private:
	UPROPERTY()
	TArray<AEmojiActor*> EmojiArray;
	
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	AMqttManager* MqttManager;
	// UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	// bool bIsSpawning = true;
	// UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	// bool bIsLoading = false;

	UPROPERTY()
	class UEmojiGameInstance* GameInstance;

	// Speeds
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float FloatSpeed = 400.f;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float SwayAmount = 2.f;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	float SwaySpeed = 2.f;

	// Sound
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USoundBase* EmojiSpawnSound;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USoundBase* CenterEmojiSpawnSound;

	bool bIsShown = true;
};


