// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MqttManager.h"
#include "Blueprint/UserWidget.h"
#include "EmojiWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class EMOJIPROJECT_API UEmojiWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Emoji")
	void SetEmojiMaterial(UMaterialInterface* NewMaterial);
	
protected:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess, BindWidget))
	UImage* EmojiImage;

private:
	UPROPERTY()
	UMaterialInterface* EmojiMaterial;
};
