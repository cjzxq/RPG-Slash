// Fill out your copyright notice in the Description page of Project Settings.
//是角色类和敌人类需要继承的

#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponents.h"//自己创建的，与.h中的前向声明对应
#include "Components/CapsuleComponent.h"
#include "Characters/CharacterType.h"//为了使用EDeathPose
//#include "Slash/DebugMacors.h"
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributeComponents>(TEXT("Attributes"));//0823
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)
{
	if (IsAlive() && Hitter)//0816
	{
		DirectionalHitReact(Hitter->GetActorLocation());//Hitter->GetActorLocation()获取击打者的位置
	}
	else
	{
		//播放死亡蒙太奇,
		Die();
	}
	// 想要在挥剑击中角色时只调用一次GetHit，回到OnBoxOverlap,可能是盒子与一个角色反复重叠多次调用GetHit，可以使用里面的ActorsToIgnore.Add(this); 
	//显示粒子 0809
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))//检查有游戏标签
	{
		CombatTarget = nullptr;//这样就不会继续攻击了
	} 
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandeMontageSection(AttackMontage,AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandeMontageSection(DeathMontage, DeathMontageSections);
	//模板包装器,将TEnumAsByte设置为EDeathPose的值
	TEnumAsByte<EDeathPose>Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
}

void ABaseCharacter::StopAttackMontage()
{
	//获取动画实例
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f,AttackMontage);
	}
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if(CombatTarget==nullptr) 	return FVector();
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
    FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();//战斗目标到敌人的位置
	TargetToMe *= WarpDistanceTarget;
	//DRAW_SPHERE(CombatTargetLocation + TargetToMe);
	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
	//return GetActorLocation();
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);//播放装备蒙太奇
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);//HitReactMontage要在蓝图中设置，设置为AM_HitReact
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	//计算向量的夹角来选择往哪边退的动画Forward，敌人的前向向量和撞击点到敌人中心的的向量ToHit来点积，最后取反余弦得到弧度，最后在转为度数 
	const FVector Forward = GetActorForwardVector();
	const FVector InpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);//向量与地面平行
	const FVector ToHit = (InpactLowered - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.0f;
	}
	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}


	PlayHitReactMontage(Section);//敌人在被击中后向左移动，但是又回到原来的位置，原因是没有利用根运动，敌人动了，胶囊体没动
	/*
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Red, 5.f);

	//加个屏幕调试信息来验证 看theta值和向量0808
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, FString::Printf(TEXT("Theta:%f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Blue, 5.f);
	*/
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	//要播放声音，用到GamePlayStatics,需要导入头文件 
	if (HitSound)//HitSound需要早蓝图中设置，否则为空0809
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	//显示粒子 0809
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint//显示血液的地方
		);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	//伤害敌人时，更新健康条控件
	if (Attributes)
	{
		Attributes->ReciveDamage(DamageAmount);
	}
}
//这里有问题 开始没写，按e键角色就不动了，也不能攻击 
void ABaseCharacter::PlayMontageSection(UAnimMontage*Montage,const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();//获取角色网格体的动画实例 获取角色 Skeletal Mesh 的动画实例（UAnimInstance）。
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandeMontageSection(UAnimMontage* Montage, TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxIndexSection = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxIndexSection);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

bool ABaseCharacter::CanAttack()
{
	return true;
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::AttackEnd()
{
	
}

void ABaseCharacter::Die()
{
	//为了避免任务死后攻击的人还在继续攻击，添加游戏标签
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}


void ABaseCharacter::SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);//武器的碰撞启用状态 访问指针 在蓝图中设置武器的碰撞启用状态0807
		EquippedWeapon->IgnoreActors.Empty();//清空数组并丢弃所有指针0808 解决盒子与一个角色反复重叠多次调用GetHit
	}
}

