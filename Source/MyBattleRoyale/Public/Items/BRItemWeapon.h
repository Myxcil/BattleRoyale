// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BRItem.h"
#include "BRItemWeapon.generated.h"

UCLASS()
class MYBATTLEROYALE_API ABRItemWeapon : public ABRItem
{
	GENERATED_BODY()

public:
	ABRItemWeapon();
	
	virtual void BeginPlay() override;
};
