// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include"Components/CapsuleComponent.h"
// Sets default values
ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;//可破坏角色不需要tick,设为false

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);//设为根组件,
	//需要配置几何集合
	GeometryCollection->SetGenerateOverlapEvents(true);//默认启用重叠事件，
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);//0810
	//有了可打破的角色，作什么？
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());//在蓝图中就会看见Capsule组件了
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);//0810


}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	//委托
	//GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &);
	
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	//在这里生成一个角色并获取击中的实例 打碎物体，出来宝藏   目前还有点问题，就是打了一个罐子把旁边的罐子的位置移动了之后，旁边的罐子没有碰撞了，还有就是打破罐子，宝藏出来在其它地方了
	//也会多次调用 出现死循环    解决方法：加一个bool变量，打碎了就为真
	if (BeBroken) return;//不会多次执行
	BeBroken = true;
	UWorld* World = GetWorld();
	if (World && TreasureClasses.Num()>0)//检查宝藏类数组元素是不是>0
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}

}

