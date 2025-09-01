// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/HitInterfaces.h" //�����ӿ�ͷ�ļ�
#include "Characters/CharacterType.h"//0816
#include "Enemy.generated.h"

//class UWidgetComponent;//��ue�д�����WidgetComoonent�� UE�Դ����࣬����Ҫ����һ����Ҫ����AttributeComponents.h������
class UHealthBarComponent;
class UPawnSensingComponent;//0821

 //�̳нӿ�,���ؼ̳�ʱ���ӿ�����������һ���̳� AEnemy �̳��˽ӿ� IHitInterfaces����ô���ǿ��԰�������һ�� IHitInterfaces ������������ͨ��ָ������ã�����Ϊ������ӿڵ���������

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemy();
	//AActor
	virtual void Tick(float DeltaTime) override;
	//0808 �ڵ��˻���ʱ�����������
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	//AActor

	//IHitInterfaces
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;	//�ӿ��еĴ��麯���޸���    ��ͼԭ���¼��ڲ�����
	//IHitInterfaces

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//<ABaseCharacter>
	virtual void Die() override;//0816
	//<ABaseCharacter>
	//Ȼ����ö�����ִ�������0816
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;//Ĭ��ΪѲ��״̬


private:
	//AI��Ϊ
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();//ÿ��Ѳ�߶�ʱ��������ִ������ص�����
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
	void StartAttackTimer();//������������ʱ��
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);//�����������ָ���İ뾶�ж������Ƿ���Ŀ�귶Χ�ڣ����򷵻�true
	void MoveToTarget(AActor* Target);
	void SpawnDefaultWeapon();
	AActor* ChoosePatrolTarget();//
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);//
	//���
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;//��Ҫ����Ĭ��������󹹽���
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon>WeaponClass;//�������������������ͼ��ӱ���

	//��̫�涯��������
	UPROPERTY(EditAnywhere)//0817
	double CombatRadius = 500.f;
	UPROPERTY(EditAnywhere)//0821
	double AttackRadius = 150.f;//�����125�����ڣ����˿�ʼ����

	//Navigation����Ѳ��
	UPROPERTY()
	class AAIController* EnemyContorller;//���˿�����0819
	//Ϊ��������ͼʵ�ֵ��˸��ٽ�ɫ����Ϊ��˽�еģ�����meta˽�з���Ϊ�棬�����Ͳ����Ƶ�public�� 0821
	//UPROPERTY(EditInstanceOnly, Category = "AI Navigation",BlueprintReadWrite,meta=(AllowPrivateAccess="true"))//Ϊ��������ͼʵ�ߵ��˸��ٽ�ɫ����Ϊ��˽�еģ�����meta˽�з���Ϊ�棬�����Ͳ����Ƶ�public��
	//��c++ʵ�ֵ��˸��ٽ�ɫ
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;//��ǰѲ��Ŀ�� ��һ��Ѳ��Ŀ������ɹ�ѡ�� һ���������Ѳ��Ŀ�꣬�ͻ���ѡ��һ���µ�Ѳ��Ŀ��
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*>PatrolTargets;
	UPROPERTY(EditAnywhere)//0817
	double PatrolRadius = 200.f;//Ѳ�߷�Χ
	FTimerHandle PatrolTimer;//��ʱѲ�ߣ�Ϊ���ö�ʱ��ִ��������Ҫһ���ص�����0820

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
