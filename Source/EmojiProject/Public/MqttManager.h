// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmojiProject/EmojiProject.h"
#include "GameFramework/Actor.h"
#include "HTTP.h"
#include "DTMqtt/DTMqtt/DTMqttClient.h"
#include "Interfaces/IHttpRequest.h"
#include "EmojiGameInstance.h"
#include "MqttManager.generated.h"

class UEmojiGameInstance;

class FMqttAsyncTask : public FNonAbandonableTask
{
public:
	FMqttAsyncTask(UEmojiGameInstance* NewGameInstance, UDTMqttClient* NewMqttClient, FString NewServerUrl)
		: GameInstance(NewGameInstance), MqttClient(NewMqttClient), ServerUrl(NewServerUrl), ClientId(FGuid::NewGuid().ToString()) {}
	~FMqttAsyncTask() {}

	void DoWork()
	{
		while (true)
		{
			MqttClient->Connect(ServerUrl, ClientId, FString("ychahn"), FString("1"), 60, bIsSuccess, ErrorMsg);

			if (bIsSuccess)
			{
				AsyncTask(ENamedThreads::GameThread, [this]
				{
					if (GameInstance->IsValidLowLevelFast())
					{
						GameInstance->LogToFile(LOGTEXT(TEXT("Success to connect")));
					}
				});
				return;
			}
			else
			{
				AsyncTask(ENamedThreads::GameThread, [this]
				{
					if (GameInstance->IsValidLowLevelFast())
						{
							GameInstance->ShowToastMessage(TEXT("연결 실패"));
							GameInstance->LogToFile(LOGTEXT(TEXT("Failed to connect")));
						}
				});
				FPlatformProcess::Sleep(3.0f);
			}
		}
	}
	FORCEINLINE TStatId GetStatId() const { RETURN_QUICK_DECLARE_CYCLE_STAT(FMqttAsyncTask, STATGROUP_ThreadPoolAsyncTasks)}

private:
	UEmojiGameInstance* GameInstance;

	UDTMqttClient* MqttClient;
	FString ServerUrl;
	FString ClientId;

	bool bIsSuccess = false;
	FString ErrorMsg;
	int32 LoopCount = 10;
};

class AEmojiManager;
UCLASS()
class EMOJIPROJECT_API AMqttManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMqttManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const TArray<FString>& GetMqttMessageArray() const;
	// Load Function
	void LoadMqttMessageArray(const TArray<FString>& NewMqttMessageArray);

	UFUNCTION(BlueprintCallable)
	void CreateMqttClient();
	UFUNCTION(BlueprintCallable)
	void ConnectToServer();
	UFUNCTION(BlueprintCallable)
	void ReconnectToServer();

	// Delegate Function
	UFUNCTION()
	void OnConnected(const FString& Cause);
	UFUNCTION()
	void OnDisconnected(const FString& Cause);
	UFUNCTION(BlueprintCallable)
	void OnMessage(const FString& Topic, const FString& Message);

	UFUNCTION()
	void ParseMessage(const FString& Message);

	UFUNCTION()
	bool RequestHTTP(const FString& URL);

	UFUNCTION()
	void CallbackHTTP(/* Emoji 종류 입력 변수 */);
	// 여기서 Emoji 종류 신호를 입력받는 함수를 구현해야할 듯

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void OnCallbackResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response,
											  bool bConnectedSuccessfully);

	// 앞서 받은 입력에 대한 Emoji를 여기서 구현해야할 듯
	// 원래는 Base64ToTexture2D 함수가 위치 

	bool HasMessage() const;

	// 이모지는 입력 받자마자 바로 실행해야 하기 때문에 여기 함수는 필요 없음 
	// void RequestEmojiData();

	// 이 부분도 바꿔야 함 
	// TArray<uint8> FStringToUint8(const FString& InString);

	const FString& GetPId() const;
private:
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	TArray<FString> MqttMessageArray;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	AEmojiManager* EmojiManager;
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	TMap<FString, FString> ConfigMap;
	UPROPERTY()
	UDTMqttClient* MqttClient;

	// Load
	bool bIsLoading = false;

	// Reconnect
	FTimerHandle ReconnectTimerHandle;

	// Data
	FString PId;
	FString Emoji;
	FString Special;
	// FString Text;
	// FString ImageURL;
	// FString CallbackURL;
	

	// Mutex
	mutable FCriticalSection DataGuard;

	// GameInstance
	class UEmojiGameInstance* GameInstance;

	// FString GetContentAsString() const;
};
