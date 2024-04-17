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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server,Reliable)
	void ServerSwitchPlayerViewToPlane();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Battle Royale|Plane")
	void ServerJumpFromPlane();

	UFUNCTION(BlueprintImplementableEvent, Category="Battle Royale|Plane")
	void OnSwitchPlayerViewToPlane();
	UFUNCTION(BlueprintImplementableEvent, Category="Battle Royale|Plane")
	void OnSwitchPlayerViewToCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Battle Royale|Plane");
	bool bIsInPlane = false;

protected:
	UFUNCTION(Client, Reliable)
	void ClientSwitchPlayerViewToPlane();
	UFUNCTION(Client, Reliable)
	void ClientSwitchPlayerViewToCharacter();
};
