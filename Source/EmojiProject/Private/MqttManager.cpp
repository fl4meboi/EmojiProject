// Fill out your copyright notice in the Description page of Project Settings.


#include "MqttManager.h"

// Sets default values
AMqttManager::AMqttManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMqttManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMqttManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

