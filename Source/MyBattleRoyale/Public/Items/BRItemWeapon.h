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

	FTransform GetMuzzleTransform() const;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastShoot();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Royale|Item")
	TObjectPtr<UParticleSystem> MuzzleParticles;

private:
	bool bUpdateMuzzle = false;
};
