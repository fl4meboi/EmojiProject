// Fill out your copyright notice in the Description page of Project Settings.


#include "MqttManager.h"

#include "EmojiManager.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "HttpManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "DTMqtt/MqttC/MQTTClient.h"
#include "DTMqtt/DTMqtt/DTMqttClient.h"
#include "EmojiGameInstance.h"

// Sets default values
AMqttManager::AMqttManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMqttManager::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = GetGameInstance<UEmojiGameInstance>();
	check(GameInstance);
	GameInstance->LogToFile(LOGTEXT(TEXT("Success to get GameInstance")));
	ConfigMap = GameInstance->GetConfigMap();

	CreateMqttClient();
	ConnectToServer();

	TActorIterator<AEmojiManager> It(GetWorld());
	EmojiManager = *It;
	check(EmojiManager);
}

void AMqttManager::LoadMqttMessageArray(const TArray<FString>& NewMqttMessageArray)
{
	FScopeLock Lock(&DataGuard);

	MqttMessageArray = NewMqttMessageArray;

	bIsLoading = false;
}

// Called every frame
void AMqttManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const TArray<FString>& AMqttManager::GetMqttMessageArray() const
{
	return MqttMessageArray;
}

void AMqttManager::CreateMqttClient()
{
	UDTMqttClient::CreateMqttClient(MqttClient);
	if (MqttClient->IsValidLowLevelFast() == false)
	{
		GameInstance->LogToFile(LOGTEXT(TEXT("Failed to Create Client")));
		GameInstance->ShowAlertMessage(TEXT("클라이언트 생성에 실패했습니다."));
		return;
	}
	MqttClient->OnConnected.AddDynamic(this, &AMqttManager::OnConnected);
	MqttClient->OnConnectionLost.AddDynamic(this, &AMqttManager::OnDisconnected);
	MqttClient->OnMessageArrived.AddDynamic(this, &AMqttManager::OnMessage);
}

void AMqttManager::ConnectToServer()
{
	FString ServerURL = *ConfigMap.Find(FString("mqtt_broker_ip")) + FString(":") + *ConfigMap.Find(FString("mqtt_broker_port"));
	FString ErrorMsg;
	bool bIsSuccess;

	(new FAutoDeleteAsyncTask<FMqttAsyncTask>(GameInstance, MqttClient, ServerURL))->StartBackgroundTask();

	return;

	MqttClient->Connect(ServerURL, FGuid::NewGuid().ToString(), FString("ychahn"), FString("1"), 60, bIsSuccess, ErrorMsg);
	if (bIsSuccess)
	{
		if (ReconnectTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(ReconnectTimerHandle);
		}

		GameInstance->LogToFile(LOGTEXT(TEXT("Connection with Mqtt broker was successful")));
		//
		// MqttClient->Subscribe(*ConfigMap.Find(FString("mqtt_broker_topic")), EDT_QualityOfService::QoS2, bIsSuccess, ErrorMsg);
		// if (bIsSuccess)
		// {
		// 	
		// 	GameInstance->LogToFile(LOGTEXT(TEXT("Success to subscribe %s topic."), **ConfigMap.Find(FString("mqtt_broker_topic"))));
		// }
		// else
		// {
		// 	GameInstance->ShowToastMessage(TEXT("구독 실패"));
		// 	GameInstance->LogToFile(LOGTEXT(TEXT("Fail to subscribe %s topic.\nError Message : %s"), **ConfigMap.Find(FString("mqtt_broker_topic")), *ErrorMsg));
		// }
	}
	else
	{
		GameInstance->ShowToastMessage(TEXT("연결 실패"));
		GameInstance->LogToFile(LOGTEXT(TEXT("AMqttManager::ConnectToServer) Connection to Mqtt broker failed \nError Message : %s"), *ErrorMsg));

		ReconnectToServer();
	}
}

void AMqttManager::ReconnectToServer()
{
	if (ReconnectTimerHandle.IsValid() == false)
	{
		GameInstance->LogToFile(LOGTEXT(TEXT("Try to reconnect to mqtt broker server")));
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.SetTimer(ReconnectTimerHandle, FTimerDelegate::CreateLambda([this]
		{
			AsyncTask(ENamedThreads::GameThread, [this]
			{
				ConnectToServer();
			});
		}), 3.f, true);
	}
}

void AMqttManager::OnConnected(const FString& Cause)
{
	GameInstance->LogToFile(LOGTEXT(TEXT("Cause : %s"), *Cause));

	bool bIsSuccess = false;
	FString ErrorMsg;
	MqttClient->Subscribe(*ConfigMap.Find(FString("mqtt_broker_topic")), EDT_QualityOfService::QoS2, bIsSuccess, ErrorMsg);
	if (bIsSuccess)
	{
		GameInstance->LogToFile(LOGTEXT(TEXT("Success to subscribe %s topic."), **ConfigMap.Find(FString("mqtt_broker_topic"))));
	}
	else
	{
		GameInstance->ShowToastMessage(TEXT("구독 실패"));
		GameInstance->LogToFile(LOGTEXT(TEXT("Fail to subscribe %s topic.\nError Message : %s"), **ConfigMap.Find(FString("mqtt_broker_topic")), *ErrorMsg));
	}
}

void AMqttManager::OnDisconnected(const FString& Cause)
{
	GameInstance->ShowToastMessage(TEXT("연결 끊김"));
	GameInstance->LogToFile(LOGTEXT(TEXT("Cause : %s"), *Cause));

	bool bIsSuccessToDisconnect = false;
	FString DisconnectErrorMessage;
	MqttClient->Disconnect(bIsSuccessToDisconnect, DisconnectErrorMessage);
	if (bIsSuccessToDisconnect)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMqttManager::OnDisconnected) Success to disconnect"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMqttManager::OnDisconnected) Failed to disconnect : %s"), *DisconnectErrorMessage);
	}

	ConnectToServer();
}

void AMqttManager::OnMessage(const FString& Topic, const FString& Message)
{
	// FScopeLock Lock(&DataGuard);	// 메시지 주고 받는 쌍방 작업 막는 코드. 여기선 필요 없음 
	GameInstance->LogToFile(LOGTEXT(TEXT("\n		Topic:%s\n		Message:%s"), *Topic, *Message));
	// MqttMessageArray.Add(Message);
	// 원래 메시지를 받고 저장하는 과정. 이번에는 저장할 핊요 없이 메시지 받자마자 바로 실행

	UE_LOG(LogTemp, Warning, TEXT("Mqtt OnMessage"));
	// 바로 반응하기 
	ParseMessage(Message);
	UE_LOG(LogTemp, Warning, TEXT("ParseMessage called"));		// ParseMessage가 끝나고 나서 찍히는 로그 

	// Parsing한 데이터를 가지고 EmojiManager에게 Spawn 요청 
}

void AMqttManager::ParseMessage(const FString& Message)
{
	PId = FString();
	Emoji = FString();			// Map으로 관리 
	Special = FString();		// true / false에 반응
	Power = FString();			// true / false에 반응
	// Text = FString();
	// ImageURL = FString();
	// CallbackURL = FString();

	UE_LOG(LogTemp, Warning, TEXT("ParseMessage call"));

	TSharedPtr<FJsonObject> MessageObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);
	FJsonSerializer::Deserialize(Reader, MessageObject);

	if (MessageObject.IsValid() == false)
	{
		GameInstance->LogToFile(LOGTEXT(TEXT("Message is not Json type")));
		return;
	}

	TSharedPtr<FJsonObject> DataObject = MessageObject->GetObjectField(TEXT("data"));
	if (DataObject.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("DataObject valid"));
		TSharedPtr<FJsonObject> HeadObject = DataObject->GetObjectField(TEXT("head"));
		if (HeadObject.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("HeadObject valid"));
			if (HeadObject->GetStringField(TEXT("apicommand")).Compare(TEXT("content")) == 0
			|| HeadObject->GetStringField(TEXT("apiaction")).Compare(TEXT("emoji")) == 0)	
			{
				UE_LOG(LogTemp, Warning, TEXT("apicommand, apiaction read"));	
				if (HeadObject->GetStringField(TEXT("apimethod")).Compare(TEXT("push")) == 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Push body called"));
					TSharedPtr<FJsonObject> BodyObject = DataObject->GetObjectField(TEXT("body"));
					if (BodyObject.IsValid())
					{
						UE_LOG(LogTemp, Warning, TEXT("BodyObject valid"));
						PId = BodyObject->GetStringField(TEXT("pid"));
						Emoji = BodyObject->GetStringField(TEXT("emoji"));
						Special = BodyObject->GetStringField(TEXT("special"));
						EmojiManager->SideOrCenter(Special.Compare("true") == 0, Emoji);

						UE_LOG(LogTemp, Warning, TEXT("Push body read"));
					}
					return;
				}

				if (HeadObject->GetStringField(TEXT("apimethod")).Compare(TEXT("power")) == 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Power body called"));
					TSharedPtr<FJsonObject> BodyObject = DataObject->GetObjectField(TEXT("body"));
					if (BodyObject.IsValid())
					{
						UE_LOG(LogTemp, Warning, TEXT("BodyObject valid"));
						PId = BodyObject->GetStringField(TEXT("pid"));
						Power = BodyObject->GetStringField(TEXT("power"));
						EmojiManager->ShowEmoji(Power.Compare("true") == 0);

						// UE_LOG(LogTemp, Warning, TEXT("Power body read"));
					}
					return;
				}
			}
		}
		else
		{
			return;
		}
	}

	return;

	// FString LeftStr;
	// FString RightStr;
	// if (Message.Split(FString("pid\":\""), &LeftStr, &RightStr))
	// {
	// 	if (RightStr.Split(FString("\""), &PId, &RightStr))
	// 	{
	// 		if (Message.Split(FString("text\":\""), &LeftStr, &RightStr))
	// 		{
	// 			// UE_LOG(LogTemp, Warning, TEXT("AMqttManager::OnMessage) Found Right String : %s"), *RightStr);
	// 	
	// 			if (RightStr.Split(FString("\""), &Text, &RightStr))
	// 			{
	// 				// UE_LOG(LogTemp, Warning, TEXT("AMqttManager::OnMessage) Text : %s"), *Text);
	//
	// 				if (RightStr.Split(FString("img\":\""), &LeftStr, &RightStr))
	// 				{
	// 					if (RightStr.Split(FString("\""), &ImageURL, &RightStr))
	// 					{
	// 						// UE_LOG(LogTemp, Warning, TEXT("AMqttManager::OnMessage) ImageURL : %s"), *ImageURL);
	//
	// 						if (RightStr.Split(FString("callback\":\""), &LeftStr, &RightStr))
	// 						{
	// 							if (RightStr.Split(FString("\""), &CallbackURL, &RightStr))
	// 							{
	// 								// UE_LOG(LogTemp, Warning, TEXT("AMqttManager::OnMessage) CallbackURL : %s"), *CallbackURL);
	// 							}
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// 	
	// }

	FString LeftStr;
	FString RightStr;
	if (Message.Split(FString("pid\":\""), &LeftStr, &RightStr))
	{
		if (RightStr.Split(FString("\""), &PId, &RightStr))
		{
			if (Message.Split(FString("text\":\""), &LeftStr, &RightStr))
			{
				// UE_LOG(LogTemp, Warning, TEXT("AMqttManager::OnMessage) Found Right String : %s"), *RightStr);
		
				if (RightStr.Split(FString("\""), &Emoji, &RightStr))
				{
					// UE_LOG(LogTemp, Warning, TEXT("AMqttManager::OnMessage) Emoji : %s"), *Text);

					if (RightStr.Split(FString("img\":\""), &LeftStr, &RightStr))
					{
						if (RightStr.Split(FString("\""), &Special, &RightStr))
						{
							// UE_LOG(LogTemp, Warning, TEXT("AMqttManager::OnMessage) Special : %s"), *ImageURL);

							// if (RightStr.Split(FString("callback\":\""), &LeftStr, &RightStr))
							// {
							// 	if (RightStr.Split(FString("\""), &CallbackURL, &RightStr))
							// 	{
							// 		// UE_LOG(LogTemp, Warning, TEXT("AMqttManager::OnMessage) CallbackURL : %s"), *CallbackURL);
							// 	}
							// }
						}
					}
				}
			}
		}
		
	}
}

bool AMqttManager::RequestHTTP(const FString& URL)
{
	if (URL.IsEmpty())
	{
		GameInstance->LogToFile(LOGTEXT(TEXT("Image Http Url is empty")));

		return false;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb("Get");
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AMqttManager::OnResponseReceived);

	return HttpRequest->ProcessRequest();
}

void AMqttManager::CallbackHTTP(/* Emoji 종류 입력 변수 */)
{
	// if (CallbackURL.IsEmpty()) return;
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");

	// HttpRequest->SetURL(CallbackURL);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("app_id", *ConfigMap.Find(FString("app_id")));
	RequestObj->SetStringField("message_pid", PId);
	// RequestObj->SetStringField("Emoji 종류 입력 받는 변수 ")		// Emoji 종류는 Mqtt로 받음 
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);
	HttpRequest->SetContentAsString(RequestBody);

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AMqttManager::OnResponseReceived);

	HttpRequest->ProcessRequest();
}

void AMqttManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	TSharedPtr<FJsonObject> ResponseObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObject);

	UE_LOG(LogTemp, Warning, TEXT("OnResponseReceived implemented"));
	
	if (ResponseObject->GetStringField(TEXT("status")).Compare(TEXT("00200")) != 0)
	{
		GameInstance->LogToFile(LOGTEXT(TEXT("Image HTTP Response status is invalid. Status is %s"), *ResponseObject->GetStringField(TEXT("status"))));
		return;
	}

	TSharedPtr<FJsonObject> ResultObject = ResponseObject->GetObjectField(FString("result"));
	if (ResultObject.IsValid())
	{
		FString EmojiName = EmojiManager->GetRandomEmojiName();
		if (!EmojiName.IsEmpty())
		{
			// EmojiManager->SetCurrentEmoji(EmojiName);
			// EmojiManager = GetWorld()->SpawnActor<AEmojiManager>();		// ?? 왜 EmojiManager를 또 소환해?

			// Special이 true이면
			bool bIsCenter = false;
			EmojiManager->SpawnEmoji(EmojiName, bIsCenter);

			UE_LOG(LogTemp, Warning, TEXT("Emoji implemented"));
		}
	}
	else
	{
		GameInstance->LogToFile(LOGTEXT(TEXT("Image HTTP Response has no 'result' field")));
	}
}

void AMqttManager::OnCallbackResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response,
											  bool bConnectedSuccessfully)
{
	TSharedPtr<FJsonObject> ResponseObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObject);
	if (ResponseObject->GetStringField(TEXT("status")).Compare(TEXT("00200")) != 0)
	{
		
		GameInstance->LogToFile(LOGTEXT(TEXT("Callabck HTTP Response status is invalid. Status is %s "), *ResponseObject->GetStringField(TEXT("status"))));
	}
}

bool AMqttManager::HasMessage() const
{
	return (bIsLoading == false) && (MqttMessageArray.Num() > 0);
}

// void AMqttManager::RequestEmojiData()	// 이 함수 필요없음 
// {	
// 	// FScopeLock Lock(&DataGuard);			// 두 작업 동시에 진행하지 않도록 lock해주는 기능 
//
// 	FString CurrentMessage = MqttMessageArray[0];
// 	MqttMessageArray.RemoveAt(0);
//
// 	ParseMessage(CurrentMessage);
//
// 	EmojiManager->SpawnEmoji();
//
// 	UE_LOG(LogTemp, Warning, TEXT("RequestEmojiData implemented"));
// }

// // 이 부분도 바꿔야 함 
// TArray<uint8> AMqttManager::FStringToUint8(const FString& InString)		// 여기도 필요 없음 
// {
// 	TArray<uint8> OutBytes;
//
// 	// Handle empty strings
// 	if (InString.Len() > 0)
// 	{
// 		FTCHARToUTF8 Converted(*InString); // Convert to UTF8
// 		OutBytes.Append(reinterpret_cast<const uint8*>(Converted.Get()), Converted.Length());
// 	}
//
// 	return OutBytes;
// }

const FString& AMqttManager::GetPId() const
{
	return PId;
}


