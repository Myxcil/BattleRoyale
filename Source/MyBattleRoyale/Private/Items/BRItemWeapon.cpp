// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BRItemWeapon.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

namespace
{
	const FName MuzzleSocket = "Muzzle";
}

ABRItemWeapon::ABRItemWeapon()
{
	ItemType = EItemType::Weapon;
	Socket = "RHand";
	AimCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("AimCamera"));
	AimCamera->SetupAttachment(SkeletalMesh);
	constexpr float GizmoScale = 0.1f;
	AimCamera->SetWorldScale3D(FVector(GizmoScale,GizmoScale,GizmoScale));
}

FTransform ABRItemWeapon::GetMuzzleTransform() const
{
	return bUpdateMuzzle ? SkeletalMesh->GetSocketTransform(MuzzleSocket) : GetActorTransform();
}

void ABRItemWeapon::MulticastShoot_Implementation()
{
	if (bUpdateMuzzle && MuzzleParticles)
	{
		const FVector Offset = FVector::ZeroVector;
		const FRotator Rotation = FRotator::ZeroRotator;
		const FVector Scale = FVector::OneVector * 0.2f;
		UGameplayStatics::SpawnEmitterAttached( MuzzleParticles, SkeletalMesh, MuzzleSocket, Offset, Rotation, Scale, EAttachLocation::KeepRelativeOffset, true);
	}
}

void ABRItemWeapon::BeginPlay()
{
	Super::BeginPlay();
	bUpdateMuzzle = SkeletalMesh && SkeletalMesh->DoesSocketExist(MuzzleSocket);
}

