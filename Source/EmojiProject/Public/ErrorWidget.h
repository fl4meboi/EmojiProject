// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ErrorWidget.generated.h"

class UTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class EMOJIPROJECT_API UErrorWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual bool Initialize() override;
public:
	UFUNCTION(BlueprintCallable)
	void ShowAlertMessage(const FString& Message);

	UFUNCTION(BlueprintCallable)
	void ShowToastMessage(const FString& Message);

	UFUNCTION(BlueprintCallable)
	void HideToastMessage();

	UFUNCTION()
	FEventReply OnToastBorderMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

private:
	// Alert Message
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UBorder* Brd_Alert;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* TB_Alert;

	// Toast Message
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UBorder* Brd_Toast;
	UPROPERTY(Meta = (AllowPrivateAccess, BindWidget))
	UTextBlock* TB_Toast;
	FTimerHandle HideToastTimerHandle;
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	float TimeToShowToastMessage = 5.f;

	UPROPERTY(Transient, Meta = (BindWidget))
	UWidgetAnimation* ShowToastMessageAnimation;
	UPROPERTY(Transient, Meta = (AllowPrivateAccess))
	UWidgetAnimation* HideToastMessageAnimation; 
};
