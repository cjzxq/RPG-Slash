// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterfaces.h" //包含接口头文件
#include "Characters/CharacterType.h"//为了使用EDeathPose
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



protected:
	 // Called when the game starts or when spawned
	 virtual void BeginPlay() override;
	 //为什么这里用override 为了击打时敌人或角色方向与击打者对立，添加了AActor* Hitter存放击打者指针
	 virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;
	 virtual void Attack();
	 UFUNCTION(BlueprintCallable)
	 void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled);//启用武器碰撞

	 void DirectionalHitReact(const FVector& ImpactPoint);// 
	 void PlayHitSound(const FVector& ImpactPoint);
	 void SpawnHitParticles(const FVector& ImpactPoint);
	 virtual void HandleDamage(float DamageAmount);

	 void PlayHitReactMontage(const FName& SectionName);
	 virtual int32 PlayAttackMontage();
	 virtual int32 PlayDeathMontage();
	 void StopAttackMontage();
	 UFUNCTION(BlueprintCallable)//蓝图可调用
	 FVector GetTranslationWarpTarget();
	 UFUNCTION(BlueprintCallable)//蓝图可调用
	 FVector GetRotationWarpTarget();//敌人旋转以面对正确目标

	 void DisableCapsule();
	 virtual bool CanAttack();//是否可以攻击的函数，判断角色状态和动作状态
	 bool IsAlive();
	 void DisableMeshCollision();
	 UFUNCTION(BlueprintCallable)//蓝图可调用
	 virtual void AttackEnd();//攻击结束函数
	 virtual void Die();
	//组件
	UPROPERTY(VisibleAnywhere)
	UAttributeComponents* Attributes;//用了一个未定义的类型，需要前向声明0812
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* EquippedWeapon;//当前装备的武器

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;//设置打中敌人的角色，后续敌人对觉角色做出反应
	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpDistanceTarget = 75.f;//变形到目标仍然保持75单位的距离

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte <EDeathPose> DeathPose;//


private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	//随机播放蒙太奇部分
	int32 PlayRandeMontageSection(UAnimMontage* Montage, TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;//敌人被击中时播放的声音0809 去GitHit()中使用 HitSound在蓝图中设置，
	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* HitParticles;//敌人被击中时显示的粒子  HitParticles在蓝图中设置
	//蒙太奇动画，变量
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;//攻击动作蒙太奇
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;//攻击动作蒙太奇0808
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DeathMontage;//死亡蒙太奇动画0816
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName>AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName>DeathMontageSections;

public:	
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }//0831
};
