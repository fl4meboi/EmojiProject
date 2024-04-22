// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
// #include "PaperFlipbookComponent.h"
#include "EmojiActor.generated.h"

UCLASS()
class EMOJIPROJECT_API AEmojiActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmojiActor();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent;
	// UPaperFlipbookComponent* FlipbookComponent;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float TargetHeight = 1100.f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float FloatSpeed = 90.f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float SwayAmount = 5.f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float SwaySpeed = 5.f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
