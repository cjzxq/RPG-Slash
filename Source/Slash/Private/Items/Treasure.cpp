// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Interfaces/PickupInterface.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);//将OtherActor转换为ASlashCharacter类型 人物靠近武器， 人物就是OtherActor
	if (PickupInterface)
	{
		//如果转换成功，说明OtherActor是ASlashCharacter类型的角色，那么就调用角色的SetOverlappingItem函数，将当前物体（this）传递给角色
		PickupInterface->AddGold(this);//需要传入一个物品指针，重叠的物品就是当前物体（this），this指向当前AItem对象
		SpawnPickupSound();
		Destroy();//销毁拾取物品
	}
}
