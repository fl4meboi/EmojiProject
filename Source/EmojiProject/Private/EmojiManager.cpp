// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiManager.h"

#include "CenterEmojiActor.h"
#include "EmojiWidgetTW.h"
#include "EmojiManagerWidget.h"
#include "MqttManager.h"
#include "EmojiGameInstance.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PanelWidget.h"
#include "Engine/Canvas.h"
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

	GameInstance = GetGameInstance<UEmojiGameInstance>();
	check(GameInstance);

	// Get MqttManager
	TActorIterator<AMqttManager> It(GetWorld());
	MqttManager = *It;
	check(MqttManager);

	// Find the EmojiWidgetTW and get its EmojiManagerWidget
	for (TActorIterator<AEmojiWidgetTW> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AEmojiWidgetTW* EmojiWidgetTw = *ActorItr;
		
		if (EmojiWidgetTw)
		{
			UE_LOG(LogTemp, Warning, TEXT("EmojiManager::BeginPlay: EmojiWidgetTW found"));
			UWidgetComponent* WidgetComp = EmojiWidgetTw->WidgetComponent;
			if (WidgetComp)
			{
				UE_LOG(LogTemp, Warning, TEXT("EmojiManager::BeginPlay: EmojiWidgetTW's WidgetComponent found"));

				// WidgetComponent에 EmojiManagerWidget 넣는 코드가 이게 맞는지 확인해보자 
				// UEmojiManagerWidget* EmojiManagerWidget = Cast<UEmojiManagerWidget>(WidgetComp->GetUserWidgetObject());
				UUserWidget* UserWidget = WidgetComp->GetUserWidgetObject();
				UE_LOG(LogTemp, Warning, TEXT("EmojiManager::BeginPlay: Get WidgetComponent"));		// 여기까지 옴 
				
				if (UserWidget)
				{
					UEmojiManagerWidget* EmojiManagerWidget = Cast<UEmojiManagerWidget>(UserWidget);		// cast 안하고 쓰는 방법 고민 EmojiWidget처럼
					
					if (EmojiManagerWidget)
					{
						EmojiCanvas = EmojiManagerWidget->EmojiCanvas;
						UE_LOG(LogTemp, Warning, TEXT("EmojiManager::BeginPlay: EmojiCanvas found in EmojiManagerWidget"));

						if (!EmojiCanvas)
						{
							UE_LOG(LogTemp, Warning, TEXT("EmojiManager::BeginPlay: EmojiCanvas not found in EmojiManagerWidget"));
						}

						// Debugging for EmojiMaterialMap
						for (const auto& Elem : EmojiTextureMap)
						{
							if (Elem.Value)
							{
								UE_LOG(LogTemp, Warning, TEXT("Texture Map Entry: Key=%s, Texture=%s"), *Elem.Key, *Elem.Value->GetName());
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("Texture Map Entry: Key=%s has a null Texture"), *Elem.Key);
							}
							return;			// Exit the loop if widget is found
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("EmojiManager::BeginPlay: EmojiManagerWidget not found in EmojiWidgetTW's WidgetComponent"));
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("EmojiManager::BeginPlay: WidgetComponent not found in EmojiWidgetTW"));
			}
		}
	}
}

// Called every frame
void AEmojiManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

FString AEmojiManager::GetRandomEmojiName()
{
	if (EmojiTextureMap.Num() > 0)
	{
		// Get all keys from the map
		TArray<FString> Keys;
		EmojiTextureMap.GetKeys(Keys);

		// Select a random key
		int32 RandomIndex = FMath::RandRange(0, Keys.Num() - 1);
		return Keys[RandomIndex];
	}

	return FString();	// Return an empty string if the map is empty
}

void AEmojiManager::SpawnEmoji(const FString& EmojiName, bool bIsCenter)
{
	if (EmojiCanvas && EmojiTextureMap.Contains(EmojiName))
	{
		UTexture* FoundEmojiTexture = EmojiTextureMap[EmojiName];
		if (!FoundEmojiTexture)
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnEmoji: Texture for %s is null"), *EmojiName);
			return;
		}
		
		this->EmojiWidget = CreateWidget<UEmojiWidget>(GetWorld(), EmojiWidgetClass);
		if (this->EmojiWidget)
		{
			this->EmojiWidget->SetEmojiTexture(FoundEmojiTexture, bIsCenter);
			UE_LOG(LogTemp, Warning, TEXT("SpawnEmoji: Setting texture for %s"), *EmojiName);
			
			FVector2D SpawnPosition = FMath::RandBool() ? LeftSpawnPoint : RightSpawnPoint;
			UCanvasPanelSlot* CanvasSlot = EmojiCanvas->AddChildToCanvas(this->EmojiWidget);
			UE_LOG(LogTemp, Warning, TEXT("SpawnEmoji: Added child to canvas"));

			if (CanvasSlot)
			{
				CanvasSlot->SetPosition(SpawnPosition);
				UE_LOG(LogTemp, Log, TEXT("SpawnEmoji: %s added to canvas at position (%f, %f)"), *EmojiName, SpawnPosition.X, SpawnPosition.Y);
			}	
			
			EmojiArray.Add(this->EmojiWidget);
			UGameplayStatics::PlaySound2D(GetWorld(), EmojiSpawnSound);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnEmoji: Emoji with name %s not found in map or EmojiManagerWidget is null"), *EmojiName);
	}
}

void AEmojiManager::CenterSpawnEmoji(const FString& EmojiName, bool bIsCenter)
{
	// Find EmojiManagerActor and get its WidgetComponent's UserWidget
	UUserWidget* EmojiManagerWidget = nullptr;
	for (TActorIterator<AEmojiWidgetTW> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AEmojiWidgetTW* EmojiWidgetTw = *ActorItr;
		if (EmojiWidgetTw)
		{
			UWidgetComponent* WidgetComp = EmojiWidgetTw->WidgetComponent;
			if (WidgetComp)
			{
				EmojiManagerWidget = WidgetComp->GetUserWidgetObject();
				UE_LOG(LogTemp, Warning, TEXT("EmojiManagerWidget found in EmojiWidgetTW's WidgetComponent"))
				break;				// Exit the loop if widget is found
			}
		}
	}
	
	if (EmojiCanvas && EmojiTextureMap.Contains(EmojiName))
	{
		UTexture* FoundEmojiTexture = EmojiTextureMap[EmojiName];
		if (FoundEmojiTexture)
		{
			this->EmojiWidget = CreateWidget<UEmojiWidget>(GetWorld(), EmojiWidgetClass);
			if (this->EmojiWidget)
			{
				this->EmojiWidget->SetEmojiTexture(FoundEmojiTexture, bIsCenter);
				UE_LOG(LogTemp, Warning, TEXT("CenterSpawnEmoji: Setting texture for %s"), *EmojiName);
				
				FVector2D SpawnPosition = CenterSpawnPoint;
				UCanvasPanelSlot* CanvasSlot = EmojiCanvas->AddChildToCanvas(this->EmojiWidget);
				if (CanvasSlot)
				{
					CanvasSlot->SetPosition(SpawnPosition);
					UE_LOG(LogTemp, Log, TEXT("SpawnEmoji: %s added to canvas at center position"), *EmojiName);
				}
				
				EmojiArray.Add(this->EmojiWidget);
				UGameplayStatics::PlaySound2D(GetWorld(), CenterEmojiSpawnSound);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to create EmojiWidget"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Emoji class for %s not found."), *EmojiName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Emoji with name %s not found in map or EmojiManagerWidget is null"), *EmojiName);
	}
}

void AEmojiManager::OnEmojiDestroyed(UEmojiWidget* DestroyedEmoji)
{
	EmojiArray.Remove(DestroyedEmoji);
}

void AEmojiManager::SideOrCenter(bool bIsCenter, const FString& EmojiName)
{
	if (bIsCenter)
	{
		CenterSpawnEmoji(EmojiName, bIsCenter);
	}
	else
	{
		SpawnEmoji(EmojiName, bIsCenter);
	}
}

void AEmojiManager::ShowEmoji(bool bShowEmoji)
{
	bIsShown = bShowEmoji;

	// for (this->EmojiWidget : EmojiArray)
	// {
	// 	if (EmojiWidget)
	// 	{
	// 		EmojiWidget->SetVisibility(bIsShown ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	// 	}
	// }

	// Find EmojiManagerActor and get its WidgetComponent's UserWidget
	UUserWidget* EmojiManagerWidget = nullptr;
	for (TActorIterator<AEmojiWidgetTW> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AEmojiWidgetTW* EmojiWidgetTw = *ActorItr;
		if (EmojiWidgetTw)
		{
			UWidgetComponent* WidgetComp = EmojiWidgetTw->WidgetComponent;
			if (WidgetComp)
			{
				EmojiManagerWidget = WidgetComp->GetUserWidgetObject();
				UE_LOG(LogTemp, Warning, TEXT("EmojiManagerWidget found in EmojiWidgetTW's WidgetComponent"))
				break;				// Exit the loop if widget is found
			}
		}
	}
	
	EmojiManagerWidget->SetVisibility(bIsShown ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

const TArray<UEmojiWidget*>& AEmojiManager::GetEmojiArray() const
{
	return EmojiArray;
}