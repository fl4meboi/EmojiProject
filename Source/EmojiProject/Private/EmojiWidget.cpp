// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiWidget.h"

#include "Components/Image.h"

void UEmojiWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!bIsCenter)
	{
		PlayAnimation(SideAnimation);
	}
	else
	{
		PlayAnimation(CenterAnimation);
	}
}

void UEmojiWidget::SetEmojiTexture(UTexture* NewTexture, bool bCenter)
{
		this->bIsCenter = bCenter;
		// bCenter = bIsCenter;
	
		if (!EmojiImage)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetEmojTexture: EmojiImage is null"));
			return;
		}

		if (!NewTexture)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetEmojTexture: NewTexture is null"));
			return;
		}
		// UE_LOG(LogTemp, Warning, TEXT("SetEmojiMaterial: Failed to set Emoji material"));

		EmojiTexture = NewTexture;			// Store material reference
		FSlateBrush Brush;
		Brush.SetResourceObject(NewTexture);
		EmojiImage->SetBrush(Brush);
		// EmojiImage->SetBrushFromSoftTexture(NewTexture);
		
		EmojiImage->SetVisibility(ESlateVisibility::Visible); 
		
		UE_LOG(LogTemp, Warning, TEXT("SetEmojiMaterial: Successfully set texture %s"), *NewTexture->GetName());
}


