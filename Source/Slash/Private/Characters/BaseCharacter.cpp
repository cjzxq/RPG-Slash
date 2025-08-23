// Fill out your copyright notice in the Description page of Project Settings.
//�ǽ�ɫ��͵�������Ҫ�̳е�

#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/AttributeComponents.h"//�Լ������ģ���.h�е�ǰ��������Ӧ
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
		AnimInstance->Montage_Play(HitReactMontage);//����װ����̫��
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);//HitReactMontageҪ����ͼ�����ã�����ΪAM_HitReact
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	//���������ļн���ѡ�����ı��˵Ķ���Forward�����˵�ǰ��������ײ���㵽�������ĵĵ�����ToHit����������ȡ�����ҵõ����ȣ������תΪ���� 
	const FVector Forward = GetActorForwardVector();
	const FVector InpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);//���������ƽ��
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


	PlayHitReactMontage(Section);//�����ڱ����к������ƶ��������ֻص�ԭ����λ�ã�ԭ����û�����ø��˶������˶��ˣ�������û��
	/*
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Red, 5.f);

	//�Ӹ���Ļ������Ϣ����֤ ��thetaֵ������0808
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
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);//��������ײ����״̬ ����ָ�� ����ͼ��������������ײ����״̬0807
		EquippedWeapon->IgnoreActors.Empty();//������鲢��������ָ��0808 ���������һ����ɫ�����ص���ε���GetHit
	}
}

