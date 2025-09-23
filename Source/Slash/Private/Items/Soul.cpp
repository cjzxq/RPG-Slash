// Fill out  your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/KismetSystemLibrary.h"//为了使用线追踪

void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const double LocationZ = GetActorLocation().Z;
	if (LocationZ > DesiredZ)
	{
		const FVector DeltaLocation = FVector(0.f, 0.f, DraftRate * DeltaTime);
		AddActorWorldOffset(DeltaLocation);
	}
}

void ASoul::BeginPlay()
{
	Super::BeginPlay();
	const FVector Start = GetActorLocation();//获取位置
	const FVector End = Start - FVector(0.f, 0.f, 2000.f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTyps;
	ObjectTyps.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TArray<AActor*>ActorToIgnore;
	ActorToIgnore.Add(GetOwner());//把灵魂所有者加入忽略数组中
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ObjectTyps,
		false,
		ActorToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true//不让线条追踪到灵魂本身
	);
	DesiredZ = HitResult.ImpactPoint.Z+50.f;
}

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//将 OtherActor转换为PickupInterface（接口）
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);//将OtherActor转换为ASlashCharacter类型 人物靠近武器， 人物就是OtherActor
	if (PickupInterface)
	{
		//如果转换成功，说明OtherActor是ASlashCharacter类型的角色，那么就调用角色的SetOverlappingItem函数，将当前物体（this）传递给角色
		PickupInterface->AddSouls(this);//需要传入一个物品指针，重叠的物品就是当前物体（this），this指向当前AItem对象
		SpawnPickupSystem();
		SpawnPickupSound();
		Destroy();//销毁灵魂
	} 

}
