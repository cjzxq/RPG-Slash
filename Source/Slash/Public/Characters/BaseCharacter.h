// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterfaces.h" //包含接口头文件
#include "BaseCharacter.generated.h"

class AWeapon;//前向声明，武器类
class UAnimMontage;//前向声明，动画蒙太奇类
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
	void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled);//启用武器碰撞

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Attack();

	// 播放蒙太奇动画
	 virtual void PlayAttackMontage();
	 void PlayHitReactMontage(const FName& SectionName);
	 void DirectionalHitReact(const FVector& ImpactPoint);// 
	 virtual bool CanAttack();//是否可以攻击的函数，判断角色状态和动作状态
	 UFUNCTION(BlueprintCallable)//蓝图可调用
	 virtual void AttackEnd();//攻击结束函数
	 virtual void Die();

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* EquippedWeapon;//当前装备的武器


	//蒙太奇动画，变量
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;//攻击动作蒙太奇
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;//攻击动作蒙太奇0808
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;//死亡蒙太奇动画0816

	//组件
	UPROPERTY(VisibleAnywhere)
	UAttributeComponents* Attributes;//用了一个未定义的类型，需要前向声明0812

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;//敌人被击中时播放的声音0809 去GitHit()中使用 HitSound在蓝图中设置，
	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	UParticleSystem* HitParticles;//敌人被击中时显示的粒子  HitParticles在蓝图中设置

public:	

};
