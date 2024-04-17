// Fill out your copyright notice in the Description page of Project Settings.


#include "BRCharacter.h"

#include "BRGameMode.h"
#include "BRPlane.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/BRItemWeapon.h"
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
	if (HasAuthority() || IsLocallyControlled())
	{
		AimRotation = GetController()->GetControlRotation();
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
	DOREPLIFETIME(ABRCharacter, CountdownToMatchStart);
	DOREPLIFETIME(ABRCharacter, HoldPose);
	DOREPLIFETIME_CONDITION(ABRCharacter, AimRotation, COND_SkipOwner);
}


void ABRCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	MulticastPlayerLanded();
}

float ABRCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HasAuthority())
	{
		Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaximumHealth);
		if (Health <= 0)
		{
			bIsAlive = false;
			MulticastPlayerDeath();
		}
	}
	ClientDamagePlayerLocally();
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABRCharacter::SetEquippedItem(ABRItem* Item)
{
	EquippedItem = Item;
	HoldPose = EquippedItem ? EquippedItem->GetHoldPose() : EHoldPose::None;
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
		UGameplayStatics::ApplyDamage(this, OutOfZoneDamageAmount, nullptr, nullptr, UDamageType::StaticClass());
	}
}

void ABRCharacter::ServerShootWeapon_Implementation()
{
	if (EquippedItem && EquippedItem->GetItemType() == EItemType::Weapon)
	{
		if (ABRItemWeapon* Weapon = Cast<ABRItemWeapon>(EquippedItem))
		{
			const FTransform MuzzleTransform = Weapon->GetMuzzleTransform();
			const FVector Forward = MuzzleTransform.GetRotation().GetForwardVector();
			const FVector StartLocation = MuzzleTransform.GetLocation() + Forward * 10.0f;
			const FVector EndLocation = StartLocation + Forward * 3000.0f;

			FHitResult LocalHitResult;
			const UWorld* World = GetWorld();
			if (World->LineTraceSingleByChannel(LocalHitResult, StartLocation, EndLocation, ECC_Visibility))
			{
				UKismetSystemLibrary::DrawDebugLine(World, StartLocation, EndLocation, FLinearColor::Red, 5);
				UKismetSystemLibrary::DrawDebugSphere(World, LocalHitResult.Location, 50.0f, 12, FLinearColor::Red, 5);
				if (LocalHitResult.GetActor())
				{
					if (ABRCharacter* Target = Cast<ABRCharacter>(LocalHitResult.GetActor()))
					{
						if (Target != this)
						{
							float LocalDamage = 5.0f;
							if (LocalHitResult.BoneName == "head")
							{
								LocalDamage *= 1.5f;
							}
					
							UGameplayStatics::ApplyPointDamage(Target, LocalDamage, StartLocation, LocalHitResult, GetController(), this, UDamageType::StaticClass());
						}
					}
				}
			}

			Weapon->MulticastShoot();
		}
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
