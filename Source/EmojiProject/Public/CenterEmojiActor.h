// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CenterEmojiActor.generated.h"

class AEmojiManager;
class UCapsuleComponent;

UCLASS()
class EMOJIPROJECT_API ACenterEmojiActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACenterEmojiActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float TargetHeight = 500.f;

	// Speeds
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float FloatSpeed = 100.f;

protected:
	FVector CurrentScale;
	FVector TargetScale;
	float ScaleSpeed;
};
