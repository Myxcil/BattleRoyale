// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BRPlane.generated.h"

UCLASS()
class MYBATTLEROYALE_API ABRPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	ABRPlane();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Battle Royale|Plane")
	void SetPlayerSpawn(const USceneComponent* Component) { PlayerSpawn = Component; }

	void SetEndPoint(const FVector& End);
	void GetPlayerSpawnPosition(FVector& OutPosition, FRotator& OutRotator) const; 

protected:
	virtual void BeginPlay() override;

	void DropPlayersFromPlane();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<USkeletalMeshComponent> PlaneMesh;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Battle Royale|Plane")
	float TravelDuration = 10.0f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Battle Royale|Plane")
	float AutoEjectPlayers = 10.0f;

	UPROPERTY()
	const USceneComponent* PlayerSpawn;

	float TravelTime;
	FVector StartPoint;
	FVector EndPoint;
	bool bPlayersEjected;
};
