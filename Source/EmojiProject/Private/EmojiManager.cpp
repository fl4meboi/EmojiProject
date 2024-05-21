// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiManager.h"

#include "CenterEmojiActor.h"
#include "MqttManager.h"
#include "EmojiGameInstance.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PanelWidget.h"
// #include "UE4Library/NotificationType.Pregenerated.h"

// Sets default values
AEmojiManager::AEmojiManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEmojiManager::BeginPlay()
{
	Super::BeginPlay();

	// for (FEmojiType& EmojiType : )

	GameInstance = GetGameInstance<UEmojiGameInstance>();
	check(GameInstance);

	// Set Config Data
	// const TMap<FString, FString>& ConfigMap = GameInstance->GetConfigMap();
	//
	// FloatSpeed = FCString::Atof(**ConfigMap.Find(FString("floatspeed")));
	// SwayAmount = FCString::Atof(**ConfigMap.Find(FString("swayamount")));
	// SwaySpeed = FCString::Atof(**ConfigMap.Find(FString("swayspeed")));

	// Get MqttManager
	TActorIterator<AMqttManager> It(GetWorld());
	MqttManager = *It;
	check(MqttManager);

	// Create the EmojiManagerWidget and add it to viewport
	if (EmojiManagerWidgetClass)
	{
		EmojiManagerWidget = CreateWidget<UUserWidget>(GetWorld(), EmojiManagerWidgetClass);
		if (EmojiManagerWidget)
		{
			EmojiManagerWidget->AddToViewport();

			// Find the CanvasPanel in the EmojiManageWidget
			EmojiCanvas = Cast<UCanvasPanel>(EmojiManagerWidget->GetWidgetFromName(TEXT("EmojiCanvas")));
			if (!EmojiCanvas)
			{
				UE_LOG(LogTemp, Error, TEXT("EmojiCanvas not found in EmojiMangerWidget."));
			}
		}
	}
}

// Called every frame
void AEmojiManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (bIsLoading || bIsSpawning) return;

	// if (MqttManager && MqttManager->HasMessage())
	// {
	// 	// bIsSpawning = true;
	//
	// 	// MqttManager->RequestEmojiData();
	// 	// Spawn 코드를 써야 함!
	// 	SpawnEmoji();
	// }
}

FString AEmojiManager::GetRandomEmojiName()
{
	if (EmojiWidgetMap.Num() > 0)
	{
		// Get all keys from the map
		TArray<FString> Keys;
		EmojiWidgetMap.GetKeys(Keys);

		// Select a random key
		int32 RandomIndex = FMath::RandRange(0, Keys.Num() - 1);
		return Keys[RandomIndex];
	}

	return FString();	// Return an empty string if the map is empty
}

void AEmojiManager::SpawnEmoji(const FString& EmojiName)
{
	if (EmojiCanvas && EmojiWidgetMap.Contains(EmojiName))
	{
		TSubclassOf<UUserWidget>* FoundEmojiClass = EmojiWidgetMap.Find(EmojiName);
		if (FoundEmojiClass && *FoundEmojiClass)
		{
			UUserWidget* EmojiWidget = CreateWidget<UUserWidget>(GetWorld(), *FoundEmojiClass);
			if (EmojiWidget)
			{
				FVector2D SpawnPosition = FMath::RandBool() ? LeftSpawnPoint : RightSpawnPoint;
				UCanvasPanelSlot* CanvasSlot = EmojiCanvas->AddChildToCanvas(EmojiWidget);
				if (CanvasSlot)
				{
					CanvasSlot->SetPosition(SpawnPosition);
				}

				EmojiArray.Add(EmojiWidget);

				// Destruction
				// EmojiWidget->OnDestroyed.BindUObject(this, &AEmojiManager::OnEmojiDestroyed);
				// OnEmojiDestroyed(EmojiWidget);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Emoji class for %s not found."), *EmojiName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Emoji with name %s not found in map or EmojiCanvas is null."), *EmojiName);
	}
}

void AEmojiManager::CenterSpawnEmoji(const FString& EmojiName)
{
	// FVector2D CenterPosition = FVector2D(400, 400);		// Center position
	// SpawnEmoji(EmojiName, CenterPosition);

	if (EmojiCanvas && EmojiWidgetMap.Contains(EmojiName))
	{
		TSubclassOf<UUserWidget>* FoundEmojiClass = EmojiWidgetMap.Find(EmojiName);
		if (FoundEmojiClass && *FoundEmojiClass)
		{
			UUserWidget* EmojiWidget = CreateWidget<UUserWidget>(GetWorld(), *FoundEmojiClass);
			if (EmojiWidget)
			{
				UCanvasPanelSlot* CanvasSlot = EmojiCanvas->AddChildToCanvas(EmojiWidget);
				if (CanvasSlot)
				{
					CanvasSlot->SetPosition(CenterSpawnPoint);
				}

				EmojiArray.Add(EmojiWidget);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Emoji class for %s not found."), *EmojiName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Emoji with name %s not found in map or EmojiCanvas is null."), *EmojiName);
	}
}

void AEmojiManager::OnEmojiDestroyed(UUserWidget* DestroyedEmoji)
{
	EmojiArray.Remove(DestroyedEmoji);
}

void AEmojiManager::SideOrCenter(bool bIsCenter, const FString& EmojiName)
{
	if (bIsCenter)
	{
		CenterSpawnEmoji(EmojiName);
	}
	else
	{
		SpawnEmoji(EmojiName);
	}
}

void AEmojiManager::ShowEmoji(bool bShowEmoji)
{
	bIsShown = bShowEmoji;
	// MqttManager에서 power true/false push 받으면
	// 돌면서 visibility
	
	// 이 함수를 BP에서 호출해서 Show 할지를 bool로 결정하자

	// for (auto Emoji : EmojiArray)
	// {
	// 	if (Emoji->IsValidLowLevelFast())
	// 	{
	// 		Emoji->SetActorHiddenInGame(!bIsShown);
	// 	}
	// }

	for (UUserWidget* EmojiWidget : EmojiArray)
	{
		if (EmojiWidget)
		{
			EmojiWidget->SetVisibility(bIsShown ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
	}
}

const TArray<UUserWidget*>& AEmojiManager::GetEmojiArray() const
{
	return EmojiArray;
}