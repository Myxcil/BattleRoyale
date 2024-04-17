// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

class ABRItem;

UCLASS()
class MYBATTLEROYALE_API AItemSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Battle Royale|Item")
	float Chance = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Royale|Item")
	TArray<TSubclassOf<ABRItem>> ChildClasses;
};
