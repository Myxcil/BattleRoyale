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
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Battle Royale|Health")
	void ServerDamagePlayerOutsideZone();

	UFUNCTION(BlueprintCallable, Category="Battle Royale|Health")
	float GetHealthPercent() const { return Health / MaximumHealth; } 
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Battle Royale|Plane")
	void OnSwitchPlayerViewToPlane();
	UFUNCTION(BlueprintImplementableEvent, Category="Battle Royale|Plane")
	void OnSwitchPlayerViewToCharacter();
	UFUNCTION(BlueprintImplementableEvent, Category="Battle Royale|Health")
	void OnPlayerOutOfZoneDamage();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category="Battle Royale|Plane");
	bool bIsInPlane = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Battle Royale|Zone");
	bool bIsInZone = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Royale|Health");
	float OutOfZoneDamageInterval = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Royale|Health");
	float OutOfZoneDamageAmount = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Royale|Health");
	float MaximumHealth = 100.0f;

	UFUNCTION(Client, Reliable)
	void ClientSwitchPlayerViewToPlane();
	UFUNCTION(Client, Reliable)
	void ClientSwitchPlayerViewToCharacter();
	UFUNCTION(Client, Unreliable)
	void ClientDamagePlayerLocally();

private:
	UPROPERTY(Replicated)
	float Health = 0;
};
