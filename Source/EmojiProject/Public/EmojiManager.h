// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "EmojiActor.h"
#include "EmojiWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "GameFramework/Actor.h"
#include "EmojiManager.generated.h"

class AMqttManager;
class AEmojiActor;

struct FEmojiData;

USTRUCT()
struct FEmojiType
{
	GENERATED_BODY()

	// UPROPERTY(EditAnywhere)
	// TArray<TSubclassOf<AEmojiActor>> EmojiClass;
};

UCLASS()
class EMOJIPROJECT_API AEmojiManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmojiManager();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Emoji")
	void SpawnEmoji(const FString& EmojiName, bool bIsCenter);

	UFUNCTION(BlueprintCallable, Category = "Emoji")
	void CenterSpawnEmoji(const FString& EmojiName, bool bIsCenter);

	void OnEmojiDestroyed(UEmojiWidget* DestroyedEmoji);

	void SideOrCenter(bool bIsCenter, const FString& EmojiName);

	UFUNCTION(BlueprintCallable, Category = "Emoji")
	void ShowEmoji(bool bShowEmoji);
	
	FString GetRandomEmojiName();
	
	const TArray<UEmojiWidget*>& GetEmojiArray() const;

protected:
	// Called when the game starts or when spawned

	// Spawn points
	// UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	// FVector2D LeftSpawnPoint = FVector2D(2420, 540);
	// UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	// FVector2D RightSpawnPoint = FVector2D(3380, 540);
	// UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	// FVector2D CenterSpawnPoint = FVector2D(2880, 540);
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector2D LeftSpawnPoint;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector2D RightSpawnPoint;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector2D CenterSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Emoji")
	TMap<FString, UTexture*> EmojiTextureMap;

	// UPROPERTY(EditAnywhere, Category = "Emoji")
	// TSubclassOf<class AEmojiWidgetTW> EmojiWidgetTWClass;

private:
	// // Inside EmojiManager in level 
	// UPROPERTY(EditAnywhere, Category = "Emoji")
	// TSubclassOf<UUserWidget> EmojiManagerWidgetClass;
	// UPROPERTY()
	// UUserWidget* EmojiManagerWidget;

	// Inside EmojiManager in level 
	UPROPERTY(EditAnywhere, Category = "Emoji")
	TSubclassOf<UEmojiWidget> EmojiWidgetClass;
	UPROPERTY()
	UEmojiWidget* EmojiWidget;

	UPROPERTY()
	UCanvasPanel* EmojiCanvas;
	
	UPROPERTY()
	TArray<UEmojiWidget*> EmojiArray;
	
	UPROPERTY(VisibleInstanceOnly, Meta = (AllowPrivateAccess))
	AMqttManager* MqttManager;

	UPROPERTY()
	class UEmojiGameInstance* GameInstance;

	// Speeds		// Not using 
	// UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	// float FloatSpeed = 400.f;
	// UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	// float SwayAmount = 2.f;
	// UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess))
	// float SwaySpeed = 2.f;

	// Sound
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USoundBase* EmojiSpawnSound;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	USoundBase* CenterEmojiSpawnSound;

	bool bIsShown = true;
};


