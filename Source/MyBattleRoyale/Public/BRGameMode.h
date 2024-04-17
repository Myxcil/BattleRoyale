// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BRZone.h"
#include "GameFramework/GameMode.h"
#include "BRGameMode.generated.h"

class ABRPlane;


UCLASS()
class MYBATTLEROYALE_API ABRGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	TConstArrayView<APlayerController*> GetPlayerControllers() const { return PlayerControllers; }
	float GetMatchStartCountdown() const;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual bool ReadyToStartMatch_Implementation() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void HandleMatchHasStarted() override;
	
	void SpawnZone();
	void SpawnPlane();

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Battle Royale|Basic")
	float CountdownToMatchStart;

	UPROPERTY(EditAnywhere, Blueprintable, Category="Battle Royale|Basic")
	int32 MinimumNumberOfPlayersForMatch = 2;

	UPROPERTY(EditAnywhere, Blueprintable, Category="Battle Royale|Basic")
	float MaxWaitingTime = 10.0f;

	UPROPERTY(EditAnywhere, Blueprintable, Category="Battle Royale|Zone")
	TSubclassOf<ABRZone> ZoneClass;

	UPROPERTY(EditAnywhere, Blueprintable, Category="Battle Royale|Zone")
	FVector ZoneSpawnTransform;

	UPROPERTY(EditAnywhere, Blueprintable, Category="Battle Royale|Zone")
	float ZoneSpawnDelay = 10.0f;

	UPROPERTY(EditAnywhere, Blueprintable, Category="Battle Royale|Plane")
	TSubclassOf<ABRPlane> PlaneClass;
	UPROPERTY(EditAnywhere, Blueprintable, Category="Battle Royale|Plane")
	FName PlaneStartTag = "PlaneStart";
	UPROPERTY(EditAnywhere, Blueprintable, Category="Battle Royale|Plane")
	FName PlaneEndTag = "PlaneEnd";

private:
	UPROPERTY()
	TArray<APlayerController*> PlayerControllers;

	FTimerHandle MaxTimeToStartHandle;
	UPROPERTY()
	TObjectPtr<ABRZone> CurrentZone;
};
