// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EmojiGameInstance.generated.h"

class AMqttManager;
class UErrorWidget;
class AEmojiActor;
class IMqttClientInterface;
class UEmojiSaveGame;
class AEmojiManager;
/**
 * 
 */
UCLASS()
class EMOJIPROJECT_API UEmojiGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;

	virtual void Shutdown() override;

	// Config
	UFUNCTION(BlueprintCallable)
	bool LoadFile(FString FileName, TArray<FString>& FileContent);

	UFUNCTION(BlueprintCallable)
	bool LoadConfig();
	UFUNCTION(BlueprintCallable)
	bool ApplyConfig();

	// Apply Config
	UFUNCTION()
	void MoveWindowFunction();

public:
	// Log Function
	void OpenLogFile();
	void LogToFile(const FString& InString);
	void MoveToNextFile();

	// Save Function
	UFUNCTION(BlueprintCallable)
	void Save();

	UFUNCTION()
	void SaveEmojiArray(const TArray<AEmojiActor*>& EmojiArray);
	UFUNCTION()
	void LoadEmojiArray(AEmojiManager* EmojiManager);

	UFUNCTION()
	void SaveMqttMessageArray(const TArray<FString>& MqttMessageArray);
	UFUNCTION()
	void LoadMqttMessageArray(AMqttManager* MqttManager);

	UEmojiSaveGame* GetSaveGame() const;

	// Getter
	const TMap<FString, FString>& GetConfigMap() const;

	const FString& GetFilePath() const;

	// Error Widget
	UFUNCTION(BlueprintCallable)
	void ShowAlertMessage(const FString& Message);
	UFUNCTION(BlueprintCallable)
	void ShowToastMessage(const FString& Message);

	UFUNCTION(BlueprintCallable)
	void InitFilePath();

private:
	IFileHandle* FileHandle;
	int32 LogFileIndex = 0;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	FString SlotName = FString("DefaultSlot");
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess))
	FString FilePath;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess))
	TMap<FString, FString> ConfigMap;

	UPROPERTY()
	UEmojiSaveGame* SaveGame;
	UPROPERTY()
	FTimerHandle MoveWindowTimerHandle;

	// Error Widget
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UErrorWidget> ErrorWidgetClass;
	UPROPERTY()
	UErrorWidget* ErrorWidget;	
};
