// Fill out your copyright notice in the Description page of Project Settings.


#include "BRGameMode.h"

#include "BRCharacter.h"
#include "BRPlane.h"
#include "Kismet/GameplayStatics.h"

float ABRGameMode::GetMatchStartCountdown() const
{
	return MaxTimeToStartHandle.IsValid() ? GetWorld()->GetTimerManager().GetTimerRemaining(MaxTimeToStartHandle) : 0;
}

void ABRGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABRGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(MaxTimeToStartHandle, this, &ABRGameMode::StartMatch, MaxWaitingTime, false );
}

void ABRGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayerControllers.AddUnique(NewPlayer);
	if (PlayerControllers.Num() >= MinimumNumberOfPlayersForMatch)
	{
		if (!HasMatchStarted())
		{
			GetWorld()->GetTimerManager().ClearTimer(MaxTimeToStartHandle);
			StartMatch();
		}
	}
}

bool ABRGameMode::ReadyToStartMatch_Implementation()
{
	return PlayerControllers.Num() >= MinimumNumberOfPlayersForMatch;
}

void ABRGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	RestartPlayer(NewPlayer);
}

void ABRGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	FTimerHandle Temp;
	GetWorld()->GetTimerManager().SetTimer(Temp, this, &ABRGameMode::SpawnZone, ZoneSpawnDelay, false);

	SpawnPlane();
}

void ABRGameMode::SpawnZone()
{
	CurrentZone = GetWorld()->SpawnActor<ABRZone>(ZoneClass, ZoneSpawnTransform, FRotator());
	if (CurrentZone)
	{
		CurrentZone->StartZoneChange();
	}
}

void ABRGameMode::SpawnPlane()
{
	UWorld* World = GetWorld();
	
	TArray<AActor*> StartPoints;
	UGameplayStatics::GetAllActorsWithTag(World, PlaneStartTag, StartPoints);
	if (StartPoints.IsEmpty())
		return;

	TArray<AActor*> EndPoints;
	UGameplayStatics::GetAllActorsWithTag(World, PlaneEndTag, EndPoints);
	if (EndPoints.IsEmpty())
		return;

	const int StartIndex = FMath::RandRange(0, StartPoints.Num()-1);
	const FVector StartPoint = StartPoints[StartIndex]->GetActorLocation();
	
	const int EndIndex = FMath::RandRange(0, EndPoints.Num()-1);
	const FVector EndPoint = EndPoints[EndIndex]->GetActorLocation();

	ABRPlane* Plane = World->SpawnActor<ABRPlane>(PlaneClass, StartPoint, FRotator());
	Plane->SetEndPoint(EndPoint);

	for(const auto& Controller : PlayerControllers)
	{
		if (Controller)
		{
			if (ABRCharacter* Character = Cast<ABRCharacter>(Controller->GetPawn()))
			{
				Character->ServerSwitchPlayerViewToPlane();
			}
		}
	}
}
