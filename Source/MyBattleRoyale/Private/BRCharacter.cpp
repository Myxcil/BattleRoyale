// Fill out your copyright notice in the Description page of Project Settings.


#include "BRCharacter.h"

#include "BRGameMode.h"
#include "BRPlane.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ABRCharacter::ABRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABRCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABRCharacter, bIsInPlane);
}

void ABRCharacter::ServerJumpFromPlane_Implementation()
{
	if (bIsInPlane)
	{
		bIsInPlane = false;
		if (const ABRPlane* Plane = Cast<ABRPlane>(UGameplayStatics::GetActorOfClass(GetWorld(), ABRPlane::StaticClass())))
		{
			FVector Pos;
			FRotator Rot;
			Plane->GetPlayerSpawnPosition(Pos, Rot);
			SetActorLocationAndRotation(Pos, Rot, false, nullptr, ETeleportType::ResetPhysics);
			ClientSwitchPlayerViewToCharacter();
		}
	}
}

void ABRCharacter::ServerSwitchPlayerViewToPlane_Implementation()
{
	if (!bIsInPlane)
	{
		bIsInPlane = true;
		ClientSwitchPlayerViewToPlane();
	}
}

void ABRCharacter::ClientSwitchPlayerViewToPlane_Implementation()
{
	OnSwitchPlayerViewToPlane();
}

void ABRCharacter::ClientSwitchPlayerViewToCharacter_Implementation()
{
	OnSwitchPlayerViewToCharacter();
}

