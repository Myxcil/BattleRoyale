// Fill out your copyright notice in the Description page of Project Settings.


#include "BRItem.h"

ABRItem::ABRItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABRItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABRItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

