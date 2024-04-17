// Fill out your copyright notice in the Description page of Project Settings.


#include "BRCharacter.h"

#include "BRGameMode.h"
#include "BRPlane.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ABRCharacter::ABRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsInZone = true;
}

void ABRCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GameMode != nullptr)
	{
		CountdownToMatchStart = GameMode->GetMatchStartCountdown();
	}
}

void ABRCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		const UWorld* World = GetWorld();
		
		Health = MaximumHealth;
		bIsInZone = true;
		bIsInPlane = false;

		FTimerHandle Temp;
		World->GetTimerManager().SetTimer(Temp, this, &ABRCharacter::ServerDamagePlayerOutsideZone, OutOfZoneDamageInterval, true);

		GameMode = Cast<ABRGameMode>(World->GetAuthGameMode());
	}
}

void ABRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABRCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABRCharacter, bIsInPlane);
	DOREPLIFETIME(ABRCharacter, bIsInZone);
	DOREPLIFETIME(ABRCharacter, Health);
	DOREPLIFETIME(ABRCharacter, bIsAlive);
	DOREPLIFETIME(ABRCharacter, EquippedItemType);
	DOREPLIFETIME(ABRCharacter, CountdownToMatchStart);
}


void ABRCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	MulticastPlayerLanded();
}

void ABRCharacter::SetEquippedItem(EItemType ItemType)
{
	EquippedItemType = ItemType;
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
			MulticastPlayerSkydiving();
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

void ABRCharacter::ServerDamagePlayerOutsideZone_Implementation()
{
	if (!bIsInZone && bIsAlive)
	{
		Health = FMath::Clamp(Health - OutOfZoneDamageAmount, 0.0f, MaximumHealth);
		if (Health <= 0)
		{
			bIsAlive = false;
			MulticastPlayerDeath();
		}
		ClientDamagePlayerLocally();
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

void ABRCharacter::ClientDamagePlayerLocally_Implementation()
{
	OnPlayerOutOfZoneDamage();
}

void ABRCharacter::MulticastPlayerDeath_Implementation()
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	check(CharacterMesh);
	CharacterMesh->SetCollisionProfileName(TEXT("Ragdoll"));
	CharacterMesh->SetSimulatePhysics(true);
}

void ABRCharacter::MulticastPlayerSkydiving_Implementation()
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	check(Movement);
	Movement->Velocity = UKismetMathLibrary::VLerp(FVector::ZeroVector, GetVelocity(), 0.1f);
	Movement->AirControl = 0.7f;
	Movement->GravityScale = 0.1f;
}

void ABRCharacter::MulticastPlayerLanded_Implementation()
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	check(Movement);
	Movement->AirControl = 0.36f;
	Movement->GravityScale = 1.75f;
}

