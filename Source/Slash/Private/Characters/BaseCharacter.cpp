// Fill out your copyright notice in the Description page of Project Settings.
//是角色类和敌人类需要继承的

#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/AttributeComponents.h"//自己创建的，与.h中的前向声明对应
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributeComponents>(TEXT("Attributes"));//0823
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::Attack()
{

}

void ABaseCharacter::PlayAttackMontage()
{

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

bool ABaseCharacter::CanAttack()
{
	return true;
}

void ABaseCharacter::AttackEnd()
{

}

void ABaseCharacter::Die()
{

}


void ABaseCharacter::SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);//武器的碰撞启用状态 访问指针 在蓝图中设置武器的碰撞启用状态0807
		EquippedWeapon->IgnoreActors.Empty();//清空数组并丢弃所有指针0808 解决盒子与一个角色反复重叠多次调用GetHit
	}
}

