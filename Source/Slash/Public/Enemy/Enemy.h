// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterfaces.h" //�����ӿ�ͷ�ļ�
#include "Characters/CharacterType.h"//0816
#include "Enemy.generated.h"

class UAttributeComponents;//0812
//class UWidgetComponent;//��ue�д�����WidgetComoonent�� UE�Դ����࣬����Ҫ����һ����Ҫ����AttributeComponents.h������
class UHealthBarComponent;
class UAnimMontage;//ǰ��������������̫����
class UPawnSensingComponent;//0821

 //�̳нӿ�,���ؼ̳�ʱ���ӿ�����������һ���̳� AEnemy �̳��˽ӿ� IHitInterfaces����ô���ǿ��԰�������һ�� IHitInterfaces ������������ͨ��ָ������ã�����Ϊ������ӿڵ���������

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

	//�ӿ��еĴ��麯���޸���    ��ͼԭ���¼��ڲ�����
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	void DirectionalHitReact(const FVector& ImpactPoint);
	//0808 �ڵ��˻���ʱ�����������
	//�˺�����
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	//���
	 UPROPERTY(VisibleAnywhere)
	 UAttributeComponents* Attributes;//����һ��δ��������ͣ���Ҫǰ������0812
	 UPROPERTY(VisibleAnywhere)
	 UHealthBarComponent* HealthBarWidget;
	 UPROPERTY(VisibleAnywhere)
	 UPawnSensingComponent* PawnSensing;//��Ҫ����Ĭ��������󹹽���

	//��̫�涯��������
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;//����������̫��0808

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;//������̫�涯��0816

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;//���˱�����ʱ���ŵ�����0809 ȥGitHit()��ʹ�� HitSound����ͼ�����ã�
	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	UParticleSystem* HitParticles;//���˱�����ʱ��ʾ������  HitParticles����ͼ������
	UPROPERTY()
	AActor* CombatTarget;//���ô��е��˵Ľ�ɫ���������˶Ծ���ɫ������Ӧ

	UPROPERTY(EditAnywhere)//0817
	double CombatRadius=500.f;

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
	void PatrolTimerFinished();//ÿ��Ѳ�߶�ʱ��������ִ������ص�����
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;//Ĭ��ΪѲ��״̬



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ������̫�涯��
	void PlayHitReactMontage(const FName& SectionName);
	void Die();//0816
	bool InTargetRange(AActor* Target, double Radius);//�����������ָ���İ뾶�ж������Ƿ���Ŀ�귶Χ�ڣ����򷵻�true
	void MoveToTarget(AActor* Target);

	//��Ҫ�����󶨵�һ��ί�У�
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);//
	//Ȼ����ö�����ִ�������0816
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;//�����ʼ�ǻ�ģ������ٵ�������ʱ����
	AActor* ChoosePatrolTarget();//

public:	


};
