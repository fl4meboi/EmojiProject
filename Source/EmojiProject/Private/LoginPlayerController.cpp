// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginPlayerController.h"

#include "EmojiGameInstance.h"
#include "HttpModule.h"
#include "EmojiProject/EmojiProject.h"
#include "Interfaces/IHttpResponse.h"

void ALoginPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);

	Login();
}

void ALoginPlayerController::Login()
{
	UEmojiGameInstance* GameInstance = GetGameInstance<UEmojiGameInstance>();

	check(GameInstance);
	const TMap<FString, FString>& ConfigMap = GameInstance->GetConfigMap();
	if (ConfigMap.IsEmpty())
	{
		GameInstance->LogToFile(LOGTEXT(TEXT("Fail to load config file.")));
		GameInstance->ShowAlertMessage(TEXT("설정 파일 읽기 실패"));
		return;
	}

	FString URL = *ConfigMap.Find(FString("license_url")) + FString("?app_id=") +
		*ConfigMap.Find(FString("app_id")) + FString("&license=") + *ConfigMap.Find(FString("license"));

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb("Get");
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ALoginPlayerController::OnLoginResponseReceived);
	HttpRequest->ProcessRequest();
}

void ALoginPlayerController::OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bConnectedSuccessfully)
{
	TSharedPtr<FJsonObject> ResponseObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObject);

	FString Result = ResponseObject->GetStringField(FString("result"));

	UEmojiGameInstance* GameInstance = GetGameInstance<UEmojiGameInstance>();
	check(GameInstance);

	// Todo: Current Logic is when work result is false, Should Change it to work when result is true
	if (Result.Compare(FString("true")) == 0)
	{
		GameInstance->LogToFile(LOGTEXT(TEXT("Success to verification license")));
		this->ClientTravel(TravelURL, TRAVEL_Absolute);
		GameInstance->LogToFile(LOGTEXT(TEXT("Success to travel level")));
	}
	else
	{
		GameInstance->ShowAlertMessage(TEXT("인증에 실패하였습니다."));
		GameInstance->LogToFile(LOGTEXT(TEXT("Fail to verification license")));
	}
}
