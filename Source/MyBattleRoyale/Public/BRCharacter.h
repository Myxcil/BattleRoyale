// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BRCharacter.generated.h"

UCLASS()
class MYBATTLEROYALE_API ABRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABRCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server,Reliable)
	void ServerSwitchPlayerViewToPlane();

	UFUNCTION(BlueprintImplementableEvent, Category="Battle Royale|Plane")
	void OnSwitchPlayerViewToPlane();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Royale|Plane");
	bool bIsInPlane = false;

protected:
	UFUNCTION(Client, Reliable)
	void ClientSwitchPlayerViewToPlane();
};
