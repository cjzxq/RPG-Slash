// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"//0823
#include "CharacterType.h"//�Լ�������ͷ�ļ��������˽�ɫ����ö�٣��ȷ�һ���޴��ͷ�ļ�Ҫ�úܶ�
#include "SlashCharacter.generated.h"

class USpringArmComponent;//ǰ�����������ɱ�����������������������
class UCameraComponent;//ǰ������������������������ʾ���ӽ�
class UGroomComponent;//ǰ����������ɫ��ë��
class AItem;//ǰ����������Ʒ��
class UAnimMontage;//ǰ��������������̫����
class AWeapon;//ǰ��������������
class USlashOverlay;

//������c++��ö�����Ͷ���
//enum CharacterState 
//{
//	Unequipped,
//	EquippedOneHandWeapon,
//	EquippedTwoHandWeapon,
//};
//��˽�е�ö�������ж����ɫ״̬����ʾ��ɫ��ǰ��װ��״̬ ����CharacterState State= Unequipped;
//������E��������ö�٣�����״̬��һ����ö����������ʾ״̬�������Ƿ�װ�����������Ƿ���ս��״̬�� ECS_�����淶��ʾCharacterState����д


UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlashCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//��Ϊ��ɫѪ����0�����ƶ�ȴ������Ծ����Ϊ��Ծ�õ��ǿ⺯����û������״̬
	virtual void Jump() override;


	//UFUNCTION(BlueprintCallable)
	//void DisableWeaponCollision();//����������ײ

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// ����Ļص�����
	void MoveForward(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void MoveRightAndLeft(float Value);//����������ƶ� ��ӳ�䣬��Ҫ����������
	void EkeyPressed();//E�������¼�������ӳ�䣬����Ҫ���������룬������һ���Եģ�����E���ͻ����һ���������
	virtual void Attack() override;//�����������󶨵������������¼�

	// ������̫�涯��
	//UFUNCTION(BlueprintCallable)//��ͼ�ɵ�����Ϊ�����������������д�ģ������麯����UFUNCTION(BlueprintCallable)���������ﲻ��
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;//������������
	virtual bool CanAttack() override;//�Ƿ���Թ����ĺ������жϽ�ɫ״̬�Ͷ���״̬
	virtual void Die() override;

	void PlayEquipMontage(const FName& SectionName);//����װ����������̫�涯��
	bool CanDisarm();//�Ƿ����ж�������ĺ������������ŵ�����
	bool CanArm();//�������ӱ����õ�����
	void Disarm();
	void Arm();

	UFUNCTION(BlueprintCallable)//��ͼ�ɵ���
	void AttachWeaponToBack();	//���������������������ж�£��ŵ�����

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();	//װ���������������ӱ����õ�����

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();//���װ�������ĺ��������ö���״̬Ϊ����

	UFUNCTION(BlueprintCallable)
	void HitRecatEnd();
private:
	bool IsUnoccupied();
	void InitializeSlashOverlay();
	void SetHUDHealth();

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;//��ɫ״̬����ʼ״̬Ϊδװ�� 
	UPROPERTY(BlueprintReadWrite,Meta=(AllowPrivateAccess="true")) //�޷�ֱ�ӽ�˽�б�����Ϊ��ͼ��д�����Ǹ����Ӹ�Metaָ������������ͼ��д�������
	EActionState ActionState = EActionState::EAS_Unoccupied;//����״̬����ʼ״̬Ϊ���� ,Ȼ��ȥcpp���ж�

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;//���ɱ�����������������������

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;//����������������ʾecho���ӽ�

	UPROPERTY(VisibleAnywhere,Category=Hair)
	UGroomComponent* Hair;//ë�������������ʾ��ɫ��ͷ��

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;//üë�����������ʾ��ɫ�ĺ���

	UPROPERTY(VisibleInstanceOnly)//����Ϸ���翴�����ֵ������ͼ�п��������ֵ
	AItem* OverlappingItem;//������������Ϊnullptr����ʾ��ɫ������һ����Ʒ�ص������Գ��Լ�����

	UPROPERTY()
	USlashOverlay* SlashOverlay;



	//��̫�涯��������
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;//װ��������̫��

public:
	//�����ص�����Ʒ ��������Ч�ʸ��ߣ�ÿ�ε��ú���������ִ�ж�Ҫ��ת���������붨��ĵط�����ֻ���ڼ�С�ĺ�����ͨ���ǻ�ȡ���������ú��� FORCEINLINEǿ������
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; } 
	//
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; } //��ȡ��ɫ״̬
	FORCEINLINE EActionState GetActionState() const { return ActionState; }//Ϊ����slashaniminstance������״̬
};
