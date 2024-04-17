// Fill out your copyright notice in the Description page of Project Settings.


#include "BRPlane.h"

#include "Kismet/KismetMathLibrary.h"

ABRPlane::ABRPlane()
{
	PrimaryActorTick.bCanEverTick = true;
	PlaneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlaneMesh"));
	SetRootComponent(PlaneMesh);
	bReplicates = true;
	TravelTime = -1.0f;
	AActor::SetReplicateMovement(true);
}

void ABRPlane::BeginPlay()
{
	Super::BeginPlay();
}

void ABRPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TravelTime >= 0 && TravelTime <= TravelDuration)
	{
		const float T = TravelTime / TravelDuration;
		const FVector Position = FMath::Lerp(StartPoint, EndPoint, T);
		SetActorLocation(Position);
		TravelTime += DeltaTime;
		if (TravelTime > TravelDuration)
		{
			Destroy();
		}
	}
}

void ABRPlane::SetEndPoint(const FVector& End)
{
	StartPoint = GetActorLocation();
	EndPoint = End;
	TravelTime = 0.0f;
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(StartPoint, EndPoint));
}
