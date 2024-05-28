// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiManagerWidget.h"

void UEmojiManagerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("EmojiManagerWidget: EmojiCanvas successfully bound"));
	
	if (!EmojiCanvas)
	{
		UE_LOG(LogTemp, Warning, TEXT("EmojiManagerWidget: EmojiCanvas not bound properly"));
	}
}
