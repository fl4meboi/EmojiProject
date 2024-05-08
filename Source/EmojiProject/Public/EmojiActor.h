// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmojiSaveGame.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
// #include "PaperFlipbookComponent.h"
#include "EmojiActor.generated.h"

class AEmojiManager;
class UCapsuleComponent;

struct FEmojiData;
UCLASS()
class EMOJIPROJECT_API AEmojiActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmojiActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetEmojiArrayIndex(const TArray<AEmojiActor*>& EmojiArray);
	int32 GetEmojiArrayIndex() const;

	// Save
	void SaveData(FEmojiData& EmojiData);
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent;
	// UPaperFlipbookComponent* FlipbookComponent;

	// Location Limit
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float TargetHeight = 1300.f;

	// Speeds
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float FloatSpeed = 400.f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float SwayAmount = 2.f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float SwaySpeed = 2.f;
	
	// 개별 변수
	int32 VariationIndex = 0;
	int32 EmojiArrayIndex = 0;

protected:
	FVector CurrentScale;
	FVector TargetScale;
	float ScaleSpeed;
};
