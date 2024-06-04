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

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/KDJ/Widget/WBP_EmojiManagerWidget_C"));		// _C 붙여야 class 찾기. 이렇게 찾으면 경로 바뀌면 못 찾을 수도 있음  
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

	if (WidgetComponent)
	{
		WidgetComponent->SetWidgetClass(EmojiManagerWidgetClass);
		WidgetComponent->SetDrawSize(FVector2D(500, 500));

		// Directly assign widget to EmojiManagerWidget
		EmojiManagerWidget = Cast<UEmojiManagerWidget>(WidgetComponent->GetUserWidgetObject());
		if (EmojiManagerWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW::BeginPlay: EmojiManagerWidget successfully created and assigned to WidgetComponent"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW::BeginPlay: Failed to create or assign EmojiManagerWidget"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW::BeginPlay: WidgetComponent not found"));
	}

	// if (EmojiManagerWidgetClass)
	// {
	// 	EmojiManagerWidget = CreateWidget<UEmojiManagerWidget>(GetWorld(), EmojiManagerWidgetClass);
	// 	UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW::BeginPlay: EmojiManagerWidget found and created"));
	// 	
	// 	if (EmojiManagerWidget)
	// 	{
	// 		WidgetComponent->SetWidget(EmojiManagerWidget);
	// 		WidgetComponent->SetDrawSize(FVector2D(500, 500));
	// 		UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW::BeginPlay: EmojiManagerWidget successfully created and assigned to WidgetComponent"));
	// 	}	
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW::BeginPlay: Failed to create EmojiManagerWidget"));
	// 	}
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("EmojiWidgetTW::BeginPlay: EmojiManagerWidgetClass is null"));
	// }
}

// Called every frame
void AEmojiWidgetTW::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

