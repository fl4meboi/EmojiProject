// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MqttManager.h"
#include "Blueprint/UserWidget.h"
#include "EmojiWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class EMOJIPROJECT_API UEmojiWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "WidgetEmoji")
	void SpawnWidgetEmoji(const FString& WidgetEmojiName);

	UFUNCTION(BlueprintCallable, Category = "WidgetEmoji")
	void CenterWidgetSpawnEmoji(const FString& WidgetEmojiName);
	
	void OnWidgetEmojiDestroyed();

	void WidgetSideOrCenter(bool bIsCenter, FString WidgetEmojiName);

	UFUNCTION(BlueprintCallable, Category = "WidgetEmoji")
	void ShowWidgetEmoji(bool bShowWidgetEmoji);

	// FString GetRandomWidgetEmojiName();

protected:

	// UPROPERTY(EditAnywhere, Category = "Emoji")
	// TMap<FString, TSubclassOf<>> WEmojiMap;

	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	AMqttManager* MqttManager;

	UPROPERTY()
	class UEmojiGameInstance* GameInstance;

	// Sound
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USoundBase* WidgetEmojiSpawnSound;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USoundBase* CenterWidgetEmojiSpawnSound;

	bool bIsShown = true;
	
};
