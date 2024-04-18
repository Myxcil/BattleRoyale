// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BRItem.h"
#include "BRItemWeapon.generated.h"

class UCameraComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Royale|Item")
	TObjectPtr<UParticleSystem> MuzzleParticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UCameraComponent> AimCamera;

private:
	bool bUpdateMuzzle = false;
};
