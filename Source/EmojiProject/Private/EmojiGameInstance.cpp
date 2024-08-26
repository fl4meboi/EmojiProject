// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiGameInstance.h"

#include "EmojiSaveGame.h"
#include "ErrorWidget.h"
#include "Blueprint/UserWidget.h"
#include "GenericPlatform/GenericWindow.h"
#include "DTMqtt/MqttCPP/async_client.h"
#include "EmojiProject/EmojiProject.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

void UEmojiGameInstance::Init()
{
	Super::Init();

	ErrorWidget = CreateWidget<UErrorWidget>(this, ErrorWidgetClass);
	check(ErrorWidget);

	if (USaveGame* LoadedSaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, 0))
	{
		SaveGame = Cast<UEmojiSaveGame>(LoadedSaveGame);
	}
	else
	{
		SaveGame = Cast<UEmojiSaveGame>(UGameplayStatics::CreateSaveGameObject(UEmojiSaveGame::StaticClass()));
	}

	check(SaveGame);

	LogFileIndex = SaveGame->LogFileIndex;

	InitFilePath();

	OpenLogFile();

	if (LoadConfig())
	{
		ApplyConfig();
		GetWorld()->GetTimerManager().SetTimer(MoveWindowTimerHandle, this, &UEmojiGameInstance::MoveWindowFunction, 1.f, true);
	}
}

void UEmojiGameInstance::Shutdown()
{
	Save();

	if (FileHandle)
	{
		delete FileHandle;
	}

	Super::Shutdown();
}

bool UEmojiGameInstance::LoadFile(FString FileName, TArray<FString>& FileContent)
{
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	if (FileManager.FileExists(*(FilePath + FileName)) == false) return false;

	return FFileHelper::LoadFileToStringArray(FileContent, *(FilePath + FileName));
}

bool UEmojiGameInstance::LoadConfig()
{
	TArray<FString> FileContentArray;
	if (LoadFile(TEXT("Config.txt"), FileContentArray) == false) return false;

	for (FString& Iter : FileContentArray)
	{
		if (Iter.IsEmpty() || Iter[0] == '#') continue;
		FString LeftStr;
		FString RightStr;
		Iter.Split(FString("="), &LeftStr, &RightStr, ESearchCase::IgnoreCase);
		ConfigMap.Add(LeftStr, RightStr);
	}

	return true;
}

bool UEmojiGameInstance::ApplyConfig()
{
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (GameUserSettings == nullptr) return false;

	// Resolution
	FString* Width = ConfigMap.Find(FString("width"));
	if (Width == nullptr) return false;
	FString* Height = ConfigMap.Find(FString("height"));
	if (Height == nullptr) return false;

	// FullScreenMode
	FString* FullScreenMode = ConfigMap.Find(FString("fullscreenmode"));
	if (FullScreenMode == nullptr) return false;

	GameUserSettings->SetScreenResolution(FIntPoint(FCString::Atoi(**Width), FCString::Atoi(**Height)));
	GameUserSettings->SetFullscreenMode((EWindowMode::Type)FCString::Atoi(**FullScreenMode));

	GameUserSettings->ApplyResolutionSettings(false);

	LogToFile(LOGTEXT(TEXT("Resolution set")));

	return true;
}

void UEmojiGameInstance::MoveWindowFunction()
{
	UEngine* Engine = GEngine;
	if (Engine != nullptr)
	{
		if (Engine->GameViewport != nullptr)
		{
			if (Engine->GameViewport->GetWindow())
			{
				// Position
				FString* XPos = ConfigMap.Find(FString("left"));
				FString* YPos = ConfigMap.Find(FString("top"));

				FVector2D WindowPosition = FVector2D(FCString::Atoi(**XPos), FCString::Atoi((**YPos)));
				Engine->GameViewport->GetWindow()->MoveWindowTo(WindowPosition);

				GetWorld()->GetTimerManager().ClearTimer(MoveWindowTimerHandle);

				LogToFile(LOGTEXT(TEXT("Window position set")));
			}
		}
	}
}

void UEmojiGameInstance::OpenLogFile()
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	FString LogFileName = FString::Printf(TEXT("LogFile%d.log"), LogFileIndex);

	// Check Directory
	PlatformFile.CreateDirectoryTree(*(FilePath + TEXT("Logs/")));

	FileHandle = PlatformFile.OpenWrite(*(FilePath + TEXT("Logs/") + LogFileName), true, true);
	if (FileHandle)
	{
		LogToFile(LOGTEXT(TEXT("Success to open log file")));
	}
	else
	{
		ShowToastMessage(TEXT("로그 열기 실패"));
	}
}

void UEmojiGameInstance::LogToFile(const FString& InString)
{
	if (FileHandle)
	{
		FString NewLine = TEXT("[") + FDateTime::Now().ToString() + TEXT("]") + InString + TEXT("\n");
		FileHandle->Write((const uint8*)TCHAR_TO_ANSI(*NewLine), NewLine.Len());

		if (FileHandle->Size() >= 100'000'000)
		{
			MoveToNextFile();
		}
	}
}

void UEmojiGameInstance::MoveToNextFile()
{
	if (FileHandle)
	{
		delete FileHandle;

		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		LogFileIndex = (LogFileIndex + 1) % 9;
		FString LogFileName = FString::Printf(TEXT("LogFile%d.log"), LogFileIndex);
		FString LogFilePath = FilePath + TEXT("Logs/");

		// Check Directory
		PlatformFile.CreateDirectoryTree(*LogFilePath);

		LogFilePath += LogFileName;

		if (PlatformFile.FileExists(*LogFilePath))
		{
			PlatformFile.DeleteFile(*LogFilePath);
		}

		FileHandle = PlatformFile.OpenWrite(*LogFilePath, true, true);
		if (FileHandle)
		{
			UE_LOG(LogTemp, Warning, TEXT("LogFile is opened"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LogFile is not opened"));
		}
	}
}

void UEmojiGameInstance::Save()
{
	if (SaveGame)
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->Pause();
		}

		SaveGame->LogFileIndex = LogFileIndex;

		UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);

		LogToFile(LOGTEXT(TEXT("Data is saved")));
	}
	else
	{
		LogToFile(LOGTEXT(TEXT("SaveData is invalid")));
	}
}

void UEmojiGameInstance::SaveEmojiArray(const TArray<AEmojiActor*>& EmojiArray)
{
	if (SaveGame)
	{
		// SaveGame->SaveEmojiArray(EmojiArray);
		UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
	}
}

void UEmojiGameInstance::LoadEmojiArray(AEmojiManager* EmojiManager)
{
	if (SaveGame)
	{
		SaveGame->LoadEmojiArray(EmojiManager);
		UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
	}
}

void UEmojiGameInstance::SaveMqttMessageArray(const TArray<FString>& MqttMessageArray)
{
	if (SaveGame)
	{
		SaveGame->SaveMqttMessageArray(MqttMessageArray);
	}
}

void UEmojiGameInstance::LoadMqttMessageArray(AMqttManager* MqttManager)
{
	if (SaveGame)
	{
		SaveGame->LoadMqttMessageArray(MqttManager);
	}
}

UEmojiSaveGame* UEmojiGameInstance::GetSaveGame() const
{
	return SaveGame;
}

const TMap<FString, FString>& UEmojiGameInstance::GetConfigMap() const
{
	return ConfigMap;
}

const FString& UEmojiGameInstance::GetFilePath() const
{
	return FilePath;
}

void UEmojiGameInstance::ShowAlertMessage(const FString& Message)
{
	if (ErrorWidget->IsInViewport() == false)
	{
		ErrorWidget->AddToViewport();
	}

	GetWorld()->GetAuthGameMode();

	ErrorWidget->ShowAlertMessage(Message);
}

void UEmojiGameInstance::ShowToastMessage(const FString& Message)
{
	if (ErrorWidget->IsInViewport() == false)
	{
		ErrorWidget->AddToViewport();
	}

	ErrorWidget->ShowToastMessage(Message);
}

void UEmojiGameInstance::InitFilePath()
{
	FilePath = FPlatformProcess::UserDir();
	FilePath += TEXT("MRAG/Interactive/Emoji/");
}
