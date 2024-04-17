// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BRItemWeapon.h"

ABRItemWeapon::ABRItemWeapon()
{
	ItemType = EItemType::Weapon;
	Socket = "RHand";
}

void ABRItemWeapon::BeginPlay()
{
	Super::BeginPlay();
}
