// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"

#include "Components/SizeBox.h"

void ULoginWidget::ShowLoginErrorMessage()
{
	SB_Toast->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
