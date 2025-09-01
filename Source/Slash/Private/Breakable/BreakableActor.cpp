// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include"Components/CapsuleComponent.h"
// Sets default values
ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;//���ƻ���ɫ����Ҫtick,��Ϊfalse

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);//��Ϊ�����,
	//��Ҫ���ü��μ���
	GeometryCollection->SetGenerateOverlapEvents(true);//Ĭ�������ص��¼���
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);//0810
	//���˿ɴ��ƵĽ�ɫ����ʲô��
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());//����ͼ�оͻῴ��Capsule�����
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);//0810


}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	//ί��
	//GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &);
	
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	//����������һ����ɫ����ȡ���е�ʵ�� �������壬��������   Ŀǰ���е����⣬���Ǵ���һ�����Ӱ��ԱߵĹ��ӵ�λ���ƶ���֮���ԱߵĹ���û����ײ�ˣ����о��Ǵ��ƹ��ӣ����س����������ط���
	//Ҳ���ε��� ������ѭ��    �����������һ��bool�����������˾�Ϊ��
	if (BeBroken) return;//������ִ��
	BeBroken = true;
	UWorld* World = GetWorld();
	if (World && TreasureClasses.Num()>0)//��鱦��������Ԫ���ǲ���>0
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}

}

