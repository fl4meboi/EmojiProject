// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiManagerWidget.h"

void UEmojiManagerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!EmojiCanvas)
	{
		UE_LOG(LogTemp, Warning, TEXT("EmojiCanvas not bound properly"));
	}
}
