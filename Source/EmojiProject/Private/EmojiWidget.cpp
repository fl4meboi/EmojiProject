// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiWidget.h"

#include "Components/Image.h"


void UEmojiWidget::SetEmojiMaterial(UMaterialInterface* NewMaterial)
{
	if (EmojiImage && NewMaterial)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(NewMaterial);
		// EmojiImage->SetBrush(Brush);
		EmojiImage->SetBrushFromMaterial(NewMaterial);
		
		UE_LOG(LogTemp, Warning, TEXT("Emoji material set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Emoji material not set"));
	}
}

void UEmojiWidget::HelloFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("HELLO"));
}
