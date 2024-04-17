// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BRItem.h"

#include "BRCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABRItem::ABRItem()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("CapsuleCollider"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

	SetRootComponent(BoxCollider);

	StaticMesh->SetupAttachment(BoxCollider);
	SkeletalMesh->SetupAttachment(BoxCollider);
	bReplicates = true;
}

void ABRItem::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority())
	{
		return;
	}
	
	if (bIsBaseItem)
	{
		if (UKismetMathLibrary::RandomBool())
		{
			const TSubclassOf<ABRItem> RandomItemClass = ChildClasses[UKismetMathLibrary::RandomIntegerInRange(0, ChildClasses.Num()-1)];
			ABRItem* NewItem = GetWorld()->SpawnActor<ABRItem>(RandomItemClass, GetActorTransform());
			NewItem->bIsBaseItem = false;
		}
		Destroy();
	}
	else
	{
		BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ABRItem::OnItemOverlapped);
	}
}

void ABRItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABRItem::OnItemOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const ABRCharacter* Character = Cast<ABRCharacter>(OtherActor))
	{
		USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
		if (Socket != NAME_None && CharacterMesh->DoesSocketExist(Socket))
		{
			SetActorEnableCollision(false);
			if (StaticMesh->GetStaticMesh())
			{
				StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
			AttachToComponent( CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
		}
		else
		{
			
		}
	}
}

