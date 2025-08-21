// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterType.h"//�Լ�������ͷ�ļ��������˽�ɫ����ö�٣��ȷ�һ���޴��ͷ�ļ�Ҫ�úܶ�
#include "SlashCharacter.generated.h"

class USpringArmComponent;//ǰ�����������ɱ�����������������������
class UCameraComponent;//ǰ������������������������ʾ���ӽ�
class UGroomComponent;//ǰ����������ɫ��ë��
class AItem;//ǰ����������Ʒ��
class UAnimMontage;//ǰ��������������̫����
class AWeapon;//ǰ��������������

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
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlashCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled);//����������ײ
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
	void Attack();//�����������󶨵������������¼�

	// ������̫�涯��
	void PlayAttackMontage();
	UFUNCTION(BlueprintCallable)//��ͼ�ɵ���
	void AttackEnd();//������������
	bool CanAttack();//�Ƿ���Թ����ĺ������жϽ�ɫ״̬�Ͷ���״̬

	void PlayEquipMontage(const FName& SectionName);//����װ����������̫�涯��
	bool CanDisarm();//�Ƿ����ж�������ĺ������������ŵ�����
	bool CanArm();//�������ӱ����õ�����

	UFUNCTION(BlueprintCallable)//��ͼ�ɵ���
	void Disarm();	//���������������������ж�£��ŵ�����

	UFUNCTION(BlueprintCallable)
	void Arm();	//װ���������������ӱ����õ�����

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();//���װ�������ĺ��������ö���״̬Ϊ����

private:

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

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* EquippedWeapon;//��ǰװ��������

	//��̫�涯��������
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;//����������̫��

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;//װ��������̫��

public:
	//�����ص�����Ʒ ��������Ч�ʸ��ߣ�ÿ�ε��ú���������ִ�ж�Ҫ��ת���������붨��ĵط�����ֻ���ڼ�С�ĺ�����ͨ���ǻ�ȡ���������ú��� FORCEINLINEǿ������
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; } 
	//
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; } //��ȡ��ɫ״̬

};
