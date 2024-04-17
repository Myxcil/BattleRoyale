// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BRItem.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon,
	Health,
	Throwable,
	Armor,
	Others,
};

UCLASS()
class MYBATTLEROYALE_API ABRItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABRItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnItemOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UBoxComponent> BoxCollider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Royale|Item")
	EItemType ItemType = EItemType::Others;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Royale|Item")
	bool bIsBaseItem = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Royale|Item")
	TArray<TSubclassOf<ABRItem>> ChildClasses;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Royale|Item")
	FName Socket = NAME_None;
};
