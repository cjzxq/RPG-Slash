// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/HitInterfaces.h" //包含接口头文件
#include "Characters/CharacterType.h"//0816
#include "Enemy.generated.h"

//class UWidgetComponent;//再ue中创建的WidgetComoonent类 UE自带的类，不需要向上一个需要创建AttributeComponents.h才能用
class UHealthBarComponent;
class UPawnSensingComponent;//0821

 //继承接口,多重继承时，接口类必须在最后一个继承 AEnemy 继承了接口 IHitInterfaces，那么我们可以把它当成一个 IHitInterfaces 类型来看待（通过指针或引用），因为它满足接口的所有条件

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemy();
	//AActor
	virtual void Tick(float DeltaTime) override;
	//0808 在敌人击中时调用这个函数
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	//AActor

	//IHitInterfaces
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;	//接口中的纯虚函数修改了    蓝图原生事件内部生成
	//IHitInterfaces

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//<ABaseCharacter>
	virtual void Die() override;//0816
	//<ABaseCharacter>
	//然后用枚举名字创建变量0816
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;//默认为巡逻状态


private:
	//AI行为
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();//每当巡逻定时器结束，执行这个回调函数
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();//函数将启动计时器
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);//这个函数根据指定的半径判断我们是否在目标范围内，是则返回true
	void MoveToTarget(AActor* Target);
	void SpawnDefaultWeapon();
	AActor* ChoosePatrolTarget();//
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);//
	//组件
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;//需要创建默认子类对象构建它
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon>WeaponClass;//用这个来生成武器，蓝图添加变量

	//蒙太奇动画，变量
	UPROPERTY(EditAnywhere)//0817
	double CombatRadius = 500.f;
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

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 3.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 5.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float PatrollingSpeed = 125.f;
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 3.f;
};
