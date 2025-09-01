// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterfaces.h" //�����ӿ�ͷ�ļ�
#include "Characters/CharacterType.h"//Ϊ��ʹ��EDeathPose
#include "BaseCharacter.generated.h"

class AWeapon;//ǰ��������������
class UAnimMontage;//ǰ��������������̫����
class UAttributeComponents;//0823
UCLASS()
class SLASH_API ABaseCharacter : public ACharacter,public IHitInterfaces
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;



protected:
	 // Called when the game starts or when spawned
	 virtual void BeginPlay() override;
	 //Ϊʲô������override Ϊ�˻���ʱ���˻��ɫ����������߶����������AActor* Hitter��Ż�����ָ��
	 virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;
	 virtual void Attack();
	 UFUNCTION(BlueprintCallable)
	 void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled);//����������ײ

	 void DirectionalHitReact(const FVector& ImpactPoint);// 
	 void PlayHitSound(const FVector& ImpactPoint);
	 void SpawnHitParticles(const FVector& ImpactPoint);
	 virtual void HandleDamage(float DamageAmount);

	 void PlayHitReactMontage(const FName& SectionName);
	 virtual int32 PlayAttackMontage();
	 virtual int32 PlayDeathMontage();
	 void StopAttackMontage();
	 UFUNCTION(BlueprintCallable)//��ͼ�ɵ���
	 FVector GetTranslationWarpTarget();
	 UFUNCTION(BlueprintCallable)//��ͼ�ɵ���
	 FVector GetRotationWarpTarget();//������ת�������ȷĿ��

	 void DisableCapsule();
	 virtual bool CanAttack();//�Ƿ���Թ����ĺ������жϽ�ɫ״̬�Ͷ���״̬
	 bool IsAlive();
	 void DisableMeshCollision();
	 UFUNCTION(BlueprintCallable)//��ͼ�ɵ���
	 virtual void AttackEnd();//������������
	 virtual void Die();
	//���
	UPROPERTY(VisibleAnywhere)
	UAttributeComponents* Attributes;//����һ��δ��������ͣ���Ҫǰ������0812
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* EquippedWeapon;//��ǰװ��������

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;//���ô��е��˵Ľ�ɫ���������˶Ծ���ɫ������Ӧ
	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpDistanceTarget = 75.f;//���ε�Ŀ����Ȼ����75��λ�ľ���

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte <EDeathPose> DeathPose;//


private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	//���������̫�沿��
	int32 PlayRandeMontageSection(UAnimMontage* Montage, TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;//���˱�����ʱ���ŵ�����0809 ȥGitHit()��ʹ�� HitSound����ͼ�����ã�
	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* HitParticles;//���˱�����ʱ��ʾ������  HitParticles����ͼ������
	//��̫�涯��������
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;//����������̫��
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;//����������̫��0808
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DeathMontage;//������̫�涯��0816
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName>AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName>DeathMontageSections;

public:	
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }//0831
};
