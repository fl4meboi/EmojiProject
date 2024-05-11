// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiSaveGame.h"

#include "EmojiManager.h"
#include "MqttManager.h"

void UEmojiSaveGame::AddCount()
{
	TestCount++;
}

int32 UEmojiSaveGame::GetCount() const
{
	return TestCount;
}

void UEmojiSaveGame::SaveEmojiArray(const TArray<AEmojiActor*>& EmojiArray)
{
	for (auto Emoji : EmojiArray)
	{
		Emoji->SetEmojiArrayIndex(EmojiArray);
	}
	for (auto Emoji : EmojiArray)
	{
		FEmojiData EmojiData;
		Emoji->SaveData(EmojiData);

		EmojiDataArray.Add(EmojiData);
	}
}

void UEmojiSaveGame::LoadEmojiArray(AEmojiManager* EmojiManager)
{
	// EmojiManager->LoadEmoji(EmojiDataArray);
	EmojiDataArray.Empty();
}

void UEmojiSaveGame::SaveMqttMessageArray(const TArray<FString>& NewMqttMessageArray)
{
	MqttMessageArray = NewMqttMessageArray;
}

void UEmojiSaveGame::LoadMqttMessageArray(AMqttManager* MqttManager)
{
	MqttManager->LoadMqttMessageArray(MqttMessageArray);
	MqttMessageArray.Empty();
}
