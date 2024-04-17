// Fill out your copyright notice in the Description page of Project Settings.


#include "BRPlane.h"

#include "BRCharacter.h"
#include "BRGameMode.h"
#include "Kismet/KismetMathLibrary.h"

ABRPlane::ABRPlane()
{
	PrimaryActorTick.bCanEverTick = true;
	PlaneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlaneMesh"));
	SetRootComponent(PlaneMesh);
	bReplicates = true;
	TravelTime = -1.0f;
	bPlayersEjected = false;
	AActor::SetReplicateMovement(true);
}

void ABRPlane::BeginPlay()
{
	Super::BeginPlay();
}

void ABRPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		if (TravelTime >= 0 && TravelTime <= TravelDuration)
		{
			const float T = TravelTime / TravelDuration;
			const FVector Position = FMath::Lerp(StartPoint, EndPoint, T);
			SetActorLocation(Position);
			TravelTime += DeltaTime;
			if (!bPlayersEjected && TravelTime >= (TravelDuration - AutoEjectPlayers))
			{
				DropPlayersFromPlane();
			}				
			if (TravelTime > TravelDuration)
			{
				Destroy();
			}
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

void ABRPlane::GetPlayerSpawnPosition(FVector& OutPosition, FRotator& OutRotator) const
{
	if (PlayerSpawn)
	{
		OutPosition = PlayerSpawn->GetComponentLocation();
		OutRotator = PlayerSpawn->GetComponentRotation();
	}
	else
	{
		OutPosition = GetActorLocation() - FVector(0,0,5000);
		OutRotator = GetActorRotation() + FRotator(0,180,0);
	}
}

void ABRPlane::DropPlayersFromPlane()
{
	if (const ABRGameMode* GameMode = Cast<ABRGameMode>(GetWorld()->GetAuthGameMode()))
	{
		for(auto& Controller : GameMode->GetPlayerControllers())
		{
			if (ABRCharacter* Character = Cast<ABRCharacter>(Controller->GetPawn()))
			{
				Character->ServerJumpFromPlane();
			}
		}
	}
	bPlayersEjected = true;
}

