// Fill out your copyright notice in the Description page of Project Settings.
//�ǽ�ɫ��͵�������Ҫ�̳е�

#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponents.h"//�Լ������ģ���.h�е�ǰ��������Ӧ
#include "Components/CapsuleComponent.h"
#include "Characters/CharacterType.h"//Ϊ��ʹ��EDeathPose
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
		DirectionalHitReact(Hitter->GetActorLocation());//Hitter->GetActorLocation()��ȡ�����ߵ�λ��
	}
	else
	{
		//����������̫��,
		Die();
	}
	// ��Ҫ�ڻӽ����н�ɫʱֻ����һ��GetHit���ص�OnBoxOverlap,�����Ǻ�����һ����ɫ�����ص���ε���GetHit������ʹ�������ActorsToIgnore.Add(this); 
	//��ʾ���� 0809
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
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))//�������Ϸ��ǩ
	{
		CombatTarget = nullptr;//�����Ͳ������������
	} 
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandeMontageSection(AttackMontage,AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandeMontageSection(DeathMontage, DeathMontageSections);
	//ģ���װ��,��TEnumAsByte����ΪEDeathPose��ֵ
	TEnumAsByte<EDeathPose>Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
}

void ABaseCharacter::StopAttackMontage()
{
	//��ȡ����ʵ��
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
    FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();//ս��Ŀ�굽���˵�λ��
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

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	//Ҫ�����������õ�GamePlayStatics,��Ҫ����ͷ�ļ� 
	if (HitSound)//HitSound��Ҫ����ͼ�����ã�����Ϊ��0809
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
	//��ʾ���� 0809
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint//��ʾѪҺ�ĵط�
		);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	//�˺�����ʱ�����½������ؼ�
	if (Attributes)
	{
		Attributes->ReciveDamage(DamageAmount);
	}
}
//���������� ��ʼûд����e����ɫ�Ͳ����ˣ�Ҳ���ܹ��� 
void ABaseCharacter::PlayMontageSection(UAnimMontage*Montage,const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();//��ȡ��ɫ������Ķ���ʵ�� ��ȡ��ɫ Skeletal Mesh �Ķ���ʵ����UAnimInstance����
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
	//Ϊ�˱����������󹥻����˻��ڼ��������������Ϸ��ǩ
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}


void ABaseCharacter::SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);//��������ײ����״̬ ����ָ�� ����ͼ��������������ײ����״̬0807
		EquippedWeapon->IgnoreActors.Empty();//������鲢��������ָ��0808 ���������һ����ɫ�����ص���ε���GetHit
	}
}

