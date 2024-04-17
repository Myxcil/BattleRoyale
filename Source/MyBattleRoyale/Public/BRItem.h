// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BRItem.generated.h"

class UCapsuleComponent;

UCLASS()
class MYBATTLEROYALE_API ABRItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABRItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UCapsuleComponent> CapsuleCollider;
};
