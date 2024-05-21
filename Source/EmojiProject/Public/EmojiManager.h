// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "EmojiActor.h"
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

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AEmojiActor>> EmojiClass;
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
	void SpawnEmoji(const FString& EmojiName);

	UFUNCTION(BlueprintCallable, Category = "Emoji")
	void CenterSpawnEmoji(const FString& EmojiName);

	void OnEmojiDestroyed(UUserWidget* DestroyedEmoji);

	void SideOrCenter(bool bIsCenter, const FString& EmojiName);

	UFUNCTION(BlueprintCallable, Category = "Emoji")
	void ShowEmoji(bool bShowEmoji);
	
	FString GetRandomEmojiName();
	
	const TArray<UUserWidget*>& GetEmojiArray() const;

protected:
	// Called when the game starts or when spawned

	// Spawn points
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector2D LeftSpawnPoint = FVector2D(500, 540);
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector2D RightSpawnPoint = FVector2D(1460, 540);
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess, MakeEditWidget))
	FVector2D CenterSpawnPoint = FVector2D(960, 540);

	UPROPERTY(EditAnywhere, Category = "Emoji")
	TMap<FString, TSubclassOf<UUserWidget>> EmojiWidgetMap;

private:
	UPROPERTY(EditAnywhere, Category = "Emoji")
	TSubclassOf<UUserWidget> EmojiManagerWidgetClass;

	UPROPERTY()
	UUserWidget* EmojiManagerWidget;

	UPROPERTY()
	UCanvasPanel* EmojiCanvas;
	
	UPROPERTY()
	TArray<UUserWidget*> EmojiArray;
	
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


