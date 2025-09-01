// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Interfaces/HitInterfaces.h"
#include "NiagaraComponent.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox")); //创建一个盒子武器碰撞体积0806
	WeaponBox->SetupAttachment(GetRootComponent()); //附加到根组件上
	//设置碰撞预设0806
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);//碰撞启用设置为无碰撞，在攻击的时候才会启用碰撞检测，攻击结束后再禁用碰撞检测
	//设置碰撞响应0806
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap); //所有通道的碰撞响应设置为重叠
	//设置单个通道的碰撞响应0806
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); //Pawn通道的碰撞响应设置为忽略
	//接下来需要处理重叠事件
	// 
	//先创建剑的起点和终点，然后在蓝图中设置它们的位置，对应剑的起点和终点
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start")); //创建一个盒子起点组件
	BoxTraceStart->SetupAttachment(GetRootComponent()); //附加到根组件上

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End")); ///创建一个盒子终点组件
	BoxTraceEnd->SetupAttachment(GetRootComponent()); //附加到根组件上

	//准备进行盒子追踪

}

void AWeapon::BeginPlay() 
{
	Super::BeginPlay();//因为重写BeginPlay()函数，所以需要调用父类的BeginPlay()函数0806
	//在盒组件发生重叠时调用OnBoxOverlap函数
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap); //绑定重叠事件到OnBoxOverlap函数
}


void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstiGator)
{
	ItemState = EItemState::EIS_Equipped; //设置物品状态为已装备  这样武器就不会做为悬浮状态了
	SetOwner(NewOwner);//所有者
	SetInstigator(NewInstiGator);//0812 发起者
	//把武器附加到指定的父组件和插槽上
	AttachMeshToSocket(InParent, InSocketName);
	PlayEquipSound();
	DisableSphereCollision();
	DeactivateEmbers();
}

void AWeapon::DeactivateEmbers()
{
	if (EmbersEffect)
	{
		EmbersEffect->Deactivate();//停用火花这个特效
	}
}

void AWeapon::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); //禁用碰撞检测
		//Sphere->SetGenerateOverlapEvents(false); //禁用重叠事件
	}
}

void AWeapon::PlayEquipSound()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation()); //播放装备武器时的声音
		//第一次装备武器时有声音，但是再后面按E键时还是会有声音，因为角色又和武器检测到了重叠，在slashcharacter.cpp中的:EkeyPressed()函数中
		// OverlappingWeapon有值了，所以会调用Equip函数 一旦在装备了武器，就应该禁用它的碰撞检测
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)//附着网格体到插槽上
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true); //创建一个附加变换规则，使用SnapToTarget规则，保持世界位置和旋转，焊接模拟物体
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName); //将武器附加到指定的父组件和插槽上
}

//在父类中，这两个函数与物体重叠时会打印物体名字，如果要调用父类函数，则用super:::OnSphereOverlap（） 作者把下面两个函数删除了
void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//以下是调试有重叠就自动捡起武器
	//ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);//把OtherActor转换为ASlashCharacter类型
	//if (SlashCharacter)
	//{
	//	// 把武器上的物品网格，附加到SlashCharacter的骨骼网格上 SnapToTarget吸附到目标上
	//	//FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	//	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,true); //创建一个附加变换规则，使用SnapToTarget规则，保持世界位置和旋转，焊接模拟物体
	//	//调用这个函数，将武器接到SlashCharacter角色的RightHnadSocket插槽上
	//	ItemMesh->AttachToComponent(SlashCharacter->GetMesh(), TransformRules, FName("RightHandSocket")); //将武器附加到角色的武器插槽上
	//}

}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

//
void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//不让敌人之间相互攻击
	if (ActorIsSameType(OtherActor)) return;
	FHitResult BoxHit; //创建一个碰撞结果变量 也就是命中结果，
	BoxTrace(BoxHit);
	if (BoxHit.GetActor()) //如果碰撞结果有Actor  一旦击中某个物体，就把击中的对象转为接口，如果说实现了这个接口或者说从这个接口继承过来的类，就可以调用接口函数
	{
		//不让敌人之间相互攻击
		if (ActorIsSameType(BoxHit.GetActor())) return;
		//伤害敌人。然后去Enemy.cpp中调用  先伤害敌人，再下面HitInterface->Execute_GetHit(BoxHit.GetActor(),BoxHit.ImpactPoint);就能正确判断有没有健康值
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);

		ExecuteGetHit(BoxHit);

		CreatFields(BoxHit.ImpactPoint);
	}
}

bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	//如何在被击中的角色上调用接口函数   HitInterface是一个指向 IHitInterfaces 接口的指针变量  
	//如果转换成功，就表示被击中的是实现了接口的ACTOR，可以调用接口函数  可以这么做，是因为敌人本身就是一个 IHitInterfaces的对象  它既是一个Actor，又是一个IHitInterfaces
	IHitInterfaces* HitInterface = Cast<IHitInterfaces>(BoxHit.GetActor()); //将被击中的Actor转换为IHitInterfaces接口类型
	if (HitInterface)
	{
		//HitInterface->GetHit(BoxHit.ImpactPoint);有了原生事件函数 需要虚幻的反射系统来处理这个接口
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint,GetOwner());//调用蓝图原生事件时，Execute_GetHit编译蓝图原生事件时，自动生成0810
	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTraceStart->GetComponentLocation(); //获取盒子起点的位置,得到的是世界空间的全局旋转，不是本地空间，本地空间用GetRelativeLocation 在蓝图中，是用getworldlocation,
	const FVector End = BoxTraceEnd->GetComponentLocation(); //获取盒子终点的位置

	//存一个被击中的角色列表，防止再次击中它们，
	TArray<AActor*>ActorsToIgnore; //创建一个忽略的Actor数组  TArray一个容器，
	ActorsToIgnore.Add(this); //将当前武器添加到忽略的Actor数组中，这样就不会检测到自己 不会击中武器本身

	for (AActor* Actor : IgnoreActors)//可以遍历忽略的角色， 
	{
		ActorsToIgnore.AddUnique(Actor);//没碰到一个角色，就不会对那个角色进行box trace了0808
	}


	//盒子追踪函数，并用T数组排除一些角色0807 进行盒子追踪，检测是否有物体与盒子碰撞
	UKismetSystemLibrary::BoxTraceSingle(
		this, //上下文对象
		Start, //起点
		End, //终点
		BoxTraceExtent, //盒子大小
		BoxTraceStart->GetComponentRotation(), //盒子旋转
		ETraceTypeQuery::TraceTypeQuery1, //追踪类型查询，这里使用TraceTypeQuery1
		false, //不进行复杂追踪
		ActorsToIgnore,
		bShowBoxDebug? EDrawDebugTrace::ForDuration: EDrawDebugTrace::None, //ForDuratio 显示盒子一段时间
		BoxHit, //碰撞结果
		true //是否返回碰撞结果
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());//武器知道击中了哪些角色0808
}

