// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmojiSaveGame.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
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

	void SetEmojiSpeed();

	void SetNewEmojiSpeed(float NewFloatSpeed, float NewSwayAmount, float NewSwaySpeed);

	void Init(int32 NewVariationIndex);

	// 원래는 SaveGame에서 EmojiArray의 index를 찾으려고 썼던 함수인데,
	// 지금은 ShowEmoji 함수에 사용하기 위해 용도 변경 
	void SetEmojiArrayIndex(const TArray<AEmojiActor*>& EmojiArray);
	int32 GetEmojiArrayIndex() const;
	
	void VisibilityOn();
	void VisibilityOff();

	// Save
	void SaveData(FEmojiData& EmojiData);
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent;
	// UPaperFlipbookComponent* FlipbookComponent;
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	// class PAPER2UPaperSpriteComponent : public UMeshComponent;
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	// UBillboardComponent* FlipbookComponent;
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	// UPaperFlipbook* PaperFlipbook;

	// Location Limit
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float TargetHeight = 5000.f;

	// Speeds
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float FloatSpeed;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float SwayAmount = 2.f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float SwaySpeed = 2.f;

	enum class EEmojiMovementType : uint8
	{ 
		Sway,
		StraightUp
	};

	EEmojiMovementType MovementType;

	// Set movement type method
	void SetMovementType(EEmojiMovementType NewType);

	UFUNCTION(BlueprintCallable)
	void DestroyEvent();
	
	// 개별 변수
	int32 VariationIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 EmojiArrayIndex = 0;

	// TArray<AEmojiActor*>* EmojiActorArrayPtr;

protected:
	FVector CurrentScale;
	FVector TargetScale;
	float ScaleSpeed;

public:
	DECLARE_DELEGATE_OneParam(FDele_AEmojiActorPtr, AEmojiActor*);
	FDele_AEmojiActorPtr OnEmojiDestroyed;
};
