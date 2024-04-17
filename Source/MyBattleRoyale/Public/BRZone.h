// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BRZone.generated.h"

USTRUCT(BlueprintType)
struct FZoneInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Royale|Zone");
	float StartDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Royale|Zone");
	float ScalingTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Royale|Zone");
	float ScaleFactor;
};

UCLASS()
class MYBATTLEROYALE_API ABRZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ABRZone();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server,Reliable)
	void StartZoneChange();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* ZoneMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Battle Royale|Zone");
	TArray<FZoneInfo> ZoneInfos;

	FVector DefaultScale;
	
	int32 ZoneInfoIndex = -1;
	float CurrentZoneTime = 0.0f;
	float CurrentScaleFactor = 1.0f;
	bool IsDelayPhase = false;
};
