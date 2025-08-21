// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h" 

/**
 * 
 */
UCLASS()
class SLASH_API ATreasure : public AItem
{
	GENERATED_BODY()
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PickupSound;//捡起物品声音  PickupSound在蓝图中设置，0810

	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;

};
