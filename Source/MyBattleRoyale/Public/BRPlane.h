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
	void SetEndPoint(const FVector& End);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	USkeletalMeshComponent* PlaneMesh;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Battle Royale|Plane")
	float TravelDuration = 10.0f;

	float TravelTime;
	FVector StartPoint;
	FVector EndPoint;
};
