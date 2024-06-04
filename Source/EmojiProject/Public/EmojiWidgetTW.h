// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmojiManagerWidget.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "EmojiWidgetTW.generated.h"



UCLASS()
class EMOJIPROJECT_API AEmojiWidgetTW : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmojiWidgetTW();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Emoji")
	UWidgetComponent* WidgetComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Emoji")
	UWidget* Widget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Emoji")
	TSubclassOf<UEmojiManagerWidget> EmojiManagerWidgetClass;
	UPROPERTY()
	UEmojiManagerWidget* EmojiManagerWidget;
};
