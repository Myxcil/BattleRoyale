// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemSpawner.h"

#include "Items/BRItem.h"
#include "Kismet/KismetMathLibrary.h"


AItemSpawner::AItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		if (Chance >= 1.0f || UKismetMathLibrary::RandomFloat() <= Chance)
		{
			const TSubclassOf<ABRItem> RandomItemClass = ChildClasses[UKismetMathLibrary::RandomIntegerInRange(0, ChildClasses.Num()-1)];
			if (ABRItem* NewItem = GetWorld()->SpawnActor<ABRItem>(RandomItemClass, GetActorTransform()))
			{
			}
		}
	}
}
