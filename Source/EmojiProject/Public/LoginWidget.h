// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class EMOJIPROJECT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowLoginErrorMessage();
	
protected:
	UPROPERTY(Meta = (BindWidget))
	USizeBox* SB_Toast;
};
