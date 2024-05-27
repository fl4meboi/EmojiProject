// Fill out your copyright notice in the Description page of Project Settings.


#include "EmojiWidgetTW.h"

// Sets default values
AEmojiWidgetTW::AEmojiWidgetTW()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/KDJ/Widget/WBP_EmojiManagerWidget"));
	if (WidgetClass.Succeeded())
	{
		EmojiManagerWidgetClass = WidgetClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW: EmojiManagerWidgetClass Successfully found"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW: EmojiManagerWidgetClass not found"));
	}
}

// Called when the game starts or when spawned
void AEmojiWidgetTW::BeginPlay()
{
	Super::BeginPlay();

	if (EmojiManagerWidgetClass)
	{
		EmojiManagerWidget = CreateWidget<UEmojiManagerWidget>(GetWorld(), EmojiManagerWidgetClass);
		UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW::BeginPlay: EmojiManagerWidget found and created"));
		
		if (EmojiManagerWidget)
		{
			WidgetComponent->SetWidget(EmojiManagerWidget);
			WidgetComponent->SetDrawSize(FVector2D(500, 500));
			UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW::BeginPlay: EmojiManagerWidget successfully created and assigned to WidgetComponent"));
		}	
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW::BeginPlay: Failed to create EmojiManagerWidget"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW::BeginPlay: EmojiManagerWidgetClass is null"));
	}
}

// Called every frame
void AEmojiWidgetTW::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

