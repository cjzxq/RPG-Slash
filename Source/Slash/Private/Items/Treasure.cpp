// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Characters/SlashCharacter.h"//包含角色类的头文件
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//父类的东西，子类不需要使用，不用super  为什么？武器那里调用了，两者的区别是什么？0810
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);//将OtherActor转换为ASlashCharacter类型 人物靠近武器， 人物就是OtherActor
	if (SlashCharacter)
	{
		//捡起硬币 播放声音
		if (PickupSound)//HitSound需要早蓝图中设置，否则为空0809
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSound,
				GetActorLocation()
			);
		}

		Destroy();//销毁拾取物品
	}
	//下一步，打破物体，出现宝藏0810 
}
