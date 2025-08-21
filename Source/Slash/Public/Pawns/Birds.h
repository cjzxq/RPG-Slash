// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
//#include"Components/CapsuleComponent.h"//������ײ�����  ����Ҫ����ͷ�ļ���ֻ��Ҫ������Ҳ����˵����ҪCapsuleComponent.h�е�ȫ�����룬ֻ��Ҫǰ������
#include "Birds.generated.h"//���ͷ�ļ�����������
class UCapsuleComponent;//ǰ�����������߱�����������࣬����Ҫ����CapsuleComponent.hͷ�ļ�
class USkeletalMeshComponent;//ǰ���������������һ���������������ͨ�����ڽ�ɫ������Ķ���
class USpringArmComponent;//ǰ�����������ɱ�����������������������
class UCameraComponent;//ǰ������������������������ʾ����ӽ�


UCLASS()
class SLASH_API ABirds : public APawn//�������������ʾ��ȥ��ǰ׺A
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABirds();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input ��ҵ�����
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);//ǰ��
	void Turn(float Value);//ת��
	void LookUp(float Value);//����
private:
	UPROPERTY(VisibleAnywhere)
	 UCapsuleComponent* Capsule;//������ײ����� ǰ������ ֻ��һ��ָ�룬��û�г�ʼ�� ����cpp�д���һ��Ĭ�ϵ��Ӷ���

	 UPROPERTY(VisibleAnywhere)
	 USkeletalMeshComponent* BirdsMesh;//��Ĺ������������ͨ�����ڽ�ɫ������Ķ��� 

	 UPROPERTY(VisibleAnywhere)
	 USpringArmComponent* SpringArm;//���ɱ�����������������������

	 UPROPERTY(VisibleAnywhere)
	 UCameraComponent* ViewCamera;//����������������ʾ����ӽ�
};
