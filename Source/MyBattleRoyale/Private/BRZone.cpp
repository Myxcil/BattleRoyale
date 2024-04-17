// Fill out your copyright notice in the Description page of Project Settings.


#include "BRZone.h"

// Sets default values
ABRZone::ABRZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZoneMesh"));
	SetRootComponent(ZoneMesh);
}

// Called when the game starts or when spawned
void ABRZone::BeginPlay()
{
	Super::BeginPlay();
	DefaultScale = ZoneMesh->GetRelativeScale3D();
	CurrentScaleFactor = 1.0f;
}

// Called every frame
void ABRZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ZoneInfoIndex >= 0 && ZoneInfoIndex < ZoneInfos.Num())
	{
		const FZoneInfo& ZoneInfo = ZoneInfos[ZoneInfoIndex];
		if (IsDelayPhase)
		{
			if (CurrentZoneTime >= ZoneInfo.StartDelay)
			{
				IsDelayPhase = false;
				CurrentZoneTime = 0.0f;
			}
		}
		else
		{
			if (CurrentZoneTime < ZoneInfo.ScalingTime)
			{
				const float T = CurrentZoneTime / ZoneInfo.ScalingTime;
				const float ScaleFactor = FMath::Lerp(CurrentScaleFactor, ZoneInfo.ScaleFactor, T);
				const FVector Scale = FVector(DefaultScale.X * ScaleFactor, DefaultScale.Y * ScaleFactor, DefaultScale.Z);
				ZoneMesh->SetRelativeScale3D(Scale);
			}
			else
			{
				IsDelayPhase = true;
				CurrentZoneTime = 0.0f;
				CurrentScaleFactor = ZoneInfo.ScaleFactor; 
				++ZoneInfoIndex;
			}
		}
		CurrentZoneTime += DeltaTime;
	}
}

void ABRZone::StartZoneChange_Implementation()
{
	ZoneInfoIndex = 0;
	CurrentZoneTime = 0;
	IsDelayPhase = true;
}

