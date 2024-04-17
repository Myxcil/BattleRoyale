// Fill out your copyright notice in the Description page of Project Settings.


#include "BRCharacter.h"

ABRCharacter::ABRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABRCharacter::ServerSwitchPlayerViewToPlane_Implementation()
{
	if (!bIsInPlane)
	{
		bIsInPlane =true;
		ClientSwitchPlayerViewToPlane();
	}
}

void ABRCharacter::ClientSwitchPlayerViewToPlane_Implementation()
{
	OnSwitchPlayerViewToPlane();
}
