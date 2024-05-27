// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "EmojiManagerWidget.generated.h"

/**
 * 
 */
UCLASS()
class EMOJIPROJECT_API UEmojiManagerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
	UCanvasPanel* EmojiCanvas;
};
