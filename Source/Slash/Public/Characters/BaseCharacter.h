// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterfaces.h" //�����ӿ�ͷ�ļ�
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

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled);//����������ײ

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Attack();

	// ������̫�涯��
	 virtual void PlayAttackMontage();
	 void PlayHitReactMontage(const FName& SectionName);
	 void DirectionalHitReact(const FVector& ImpactPoint);// 
	 virtual bool CanAttack();//�Ƿ���Թ����ĺ������жϽ�ɫ״̬�Ͷ���״̬
	 UFUNCTION(BlueprintCallable)//��ͼ�ɵ���
	 virtual void AttackEnd();//������������
	 virtual void Die();

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* EquippedWeapon;//��ǰװ��������


	//��̫�涯��������
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;//����������̫��
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;//����������̫��0808
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;//������̫�涯��0816

	//���
	UPROPERTY(VisibleAnywhere)
	UAttributeComponents* Attributes;//����һ��δ��������ͣ���Ҫǰ������0812

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;//���˱�����ʱ���ŵ�����0809 ȥGitHit()��ʹ�� HitSound����ͼ�����ã�
	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	UParticleSystem* HitParticles;//���˱�����ʱ��ʾ������  HitParticles����ͼ������

public:	

};
