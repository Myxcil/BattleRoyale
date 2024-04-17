// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Items/BRItem.h"
#include "BRCharacter.generated.h"

class ABRGameMode;

UCLASS()
class MYBATTLEROYALE_API ABRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABRCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Landed(const FHitResult& Hit) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(Server,Reliable)
	void ServerSwitchPlayerViewToPlane();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Battle Royale|Plane")
	void ServerJumpFromPlane();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Battle Royale|Health")
	void ServerDamagePlayerOutsideZone();
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void ServerShootWeapon();

	UFUNCTION(BlueprintCallable, Category="Battle Royale|Health")
	float GetHealthPercent() const { return Health / MaximumHealth; } 

	void SetEquippedItem(ABRItem* Item);
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Battle Royale|Player")
	void OnSwitchPlayerViewToPlane();
	UFUNCTION(BlueprintImplementableEvent, Category="Battle Royale|Player")
	void OnSwitchPlayerViewToCharacter();
	UFUNCTION(BlueprintImplementableEvent, Category="Battle Royale|Health")
	void OnPlayerOutOfZoneDamage();

	UFUNCTION(Client, Reliable)
	void ClientSwitchPlayerViewToPlane();
	UFUNCTION(Client, Reliable)
	void ClientSwitchPlayerViewToCharacter();
	UFUNCTION(Client, Unreliable)
	void ClientDamagePlayerLocally();
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayerDeath();
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayerSkydiving();
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayerLanded();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category="Battle Royale|Player");
	float CountdownToMatchStart = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category="Battle Royale|Player");
	bool bIsInPlane = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Battle Royale|Player");
	bool bIsInZone = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Battle Royale|Player");
	bool bIsAlive = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Battle Royale|Player");
	EHoldPose HoldPose = EHoldPose::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Royale|Health");
	float OutOfZoneDamageInterval = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Royale|Health");
	float OutOfZoneDamageAmount = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Battle Royale|Health");
	float MaximumHealth = 100.0f;

private:
	UPROPERTY()
	TObjectPtr<ABRGameMode> GameMode;
	UPROPERTY();
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	TObjectPtr<ABRItem> EquippedItem;
	UPROPERTY(Replicated)
	float Health = 0;
};
