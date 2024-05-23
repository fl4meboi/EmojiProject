// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiWidget.h"

#include "Components/Image.h"

void UEmojiWidget::SetEmojiMaterial(UMaterialInterface* NewMaterial)
{
		if (!EmojiImage)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetEmojiMaterial: EmojiImage is null"));
			return;
		}

		if (!NewMaterial)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetEmojiMaterial: NewMaterial is null"));
			return;
		}
		// UE_LOG(LogTemp, Warning, TEXT("SetEmojiMaterial: Failed to set Emoji material"));

		EmojiMaterial = NewMaterial;			// Store material reference
		FSlateBrush Brush;
		Brush.SetResourceObject(NewMaterial);
		// EmojiImage->SetBrush(Brush);
		EmojiImage->SetBrushFromMaterial(NewMaterial);
		EmojiImage->SetVisibility(ESlateVisibility::Visible); 
		
		UE_LOG(LogTemp, Warning, TEXT("SetEmojiMaterial: Successfully set material %s"), *NewMaterial->GetName());

	
}
