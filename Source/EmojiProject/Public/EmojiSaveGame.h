// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EmojiSaveGame.generated.h"

/**
 * 
 */

class AEmojiActor;

USTRUCT()
struct FEmojiData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 VariationIndex;
	UPROPERTY()
	FTransform Transform;

	void Print()
	{
		UE_LOG(LogTemp, Warning, TEXT("VariationIndex : %s Transform %s"), VariationIndex, *Transform.ToString());
	}
};

UCLASS()
class EMOJIPROJECT_API UEmojiSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	void AddCount();

	int32 GetCount() const;

	void SaveEmojiArray(const TArray<AEmojiActor*>& EmojiArray);

	void LoadEmojiArray(class AEmojiManager* EmojiManager);

	void SaveMqttMessageArray(const TArray<FString>& NewMqttMessageArray);

	void LoadMqttMessageArray(class AMqttManager* MqttManager);

	// Property
	UPROPERTY()
	int32 LogFileIndex = 0;

private:
	UPROPERTY(Meta = (AllowPrivateAccess))
	TArray<FString> MqttMessageArray;

	UPROPERTY(Meta = (AllowPrivateAccess))
	TArray<FEmojiData> EmojiDataArray;
	UPROPERTY(Meta = (AllowPrivateAccess))
	int32 TestCount = 0;
};
