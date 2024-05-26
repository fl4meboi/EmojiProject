// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiWidgetTW.h"

// Sets default values
AEmojiWidgetTW::AEmojiWidgetTW()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	// WidgetComponent->SetWidgetSpace()
}

// Called when the game starts or when spawned
void AEmojiWidgetTW::BeginPlay()
{
	Super::BeginPlay();

	if (EmojiManagerWidgetClass)
	{
		EmojiManagerWidget = CreateWidget<UUserWidget>(GetWorld(), EmojiManagerWidgetClass);
		if (EmojiManagerWidget)
		{
			WidgetComponent->SetWidget(EmojiManagerWidget);
		}
	}
}

// Called every frame
void AEmojiWidgetTW::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

