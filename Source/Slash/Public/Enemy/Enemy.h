// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterfaces.h" //包含接口头文件
#include "Characters/CharacterType.h"//0816
#include "Enemy.generated.h"

class UAttributeComponents;//0812
//class UWidgetComponent;//再ue中创建的WidgetComoonent类 UE自带的类，不需要向上一个需要创建AttributeComponents.h才能用
class UHealthBarComponent;
class UAnimMontage;//前向声明，动画蒙太奇类
class UPawnSensingComponent;//0821

 //继承接口,多重继承时，接口类必须在最后一个继承 AEnemy 继承了接口 IHitInterfaces，那么我们可以把它当成一个 IHitInterfaces 类型来看待（通过指针或引用），因为它满足接口的所有条件

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterfaces
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CheckPatrolTarget();
	void CheckCombatTarget();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//接口中的纯虚函数修改了    蓝图原生事件内部生成
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	void DirectionalHitReact(const FVector& ImpactPoint);
	//0808 在敌人击中时调用这个函数
	//伤害敌人
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	//组件
	 UPROPERTY(VisibleAnywhere)
	 UAttributeComponents* Attributes;//用了一个未定义的类型，需要前向声明0812
	 UPROPERTY(VisibleAnywhere)
	 UHealthBarComponent* HealthBarWidget;
	 UPROPERTY(VisibleAnywhere)
	 UPawnSensingComponent* PawnSensing;//需要创建默认子类对象构建它

	//蒙太奇动画，变量
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;//攻击动作蒙太奇0808

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;//死亡蒙太奇动画0816

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;//敌人被击中时播放的声音0809 去GitHit()中使用 HitSound在蓝图中设置，
	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	UParticleSystem* HitParticles;//敌人被击中时显示的粒子  HitParticles在蓝图中设置
	UPROPERTY()
	AActor* CombatTarget;//设置打中敌人的角色，后续敌人对觉角色做出反应

	UPROPERTY(EditAnywhere)//0817
	double CombatRadius=500.f;

	UPROPERTY(EditAnywhere)//0821
	double AttackRadius = 150.f;//如果在125距离内，敌人开始攻击


	//Navigation敌人巡航
	UPROPERTY()
	class AAIController* EnemyContorller;//敌人控制器0819
	//为了在用蓝图实现敌人跟踪角色，因为是私有的，设置meta私有访问为真，这样就不用移到public中 0821
	//UPROPERTY(EditInstanceOnly, Category = "AI Navigation",BlueprintReadWrite,meta=(AllowPrivateAccess="true"))//为了在用蓝图实线敌人跟踪角色，因为是私有的，设置meta私有访问为真，这样就不用移到public中
	//用c++实现敌人跟踪角色
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;//当前巡逻目标 有一个巡逻目标数组可供选择 一旦到达这个巡逻目标，就会有选择一个新的巡逻目标
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*>PatrolTargets;
	UPROPERTY(EditAnywhere)//0817
	double PatrolRadius = 200.f;//巡逻范围
	FTimerHandle PatrolTimer;//定时巡逻，为了让定时器执行任务，需要一个回调函数0820
	void PatrolTimerFinished();//每当巡逻定时器结束，执行这个回调函数
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;//默认为巡逻状态



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 播放蒙太奇动画
	void PlayHitReactMontage(const FName& SectionName);
	void Die();//0816
	bool InTargetRange(AActor* Target, double Radius);//这个函数根据指定的半径判断我们是否在目标范围内，是则返回true
	void MoveToTarget(AActor* Target);

	//需要将它绑定到一个委托，
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);//
	//然后用枚举名字创建变量0816
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;//敌人最开始是活的，可以再敌人死亡时设置
	AActor* ChoosePatrolTarget();//

public:	


};
