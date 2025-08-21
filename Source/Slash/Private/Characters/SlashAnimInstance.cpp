// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	//��Ϊ����д�����ø���İ汾��ȷ��������κβ�������ִ��
	Super::NativeInitializeAnimation();

	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());//��Pawnת��ΪSlashCharacter��
	if (SlashCharacter)
	{
		//��ȡ��ɫ���ƶ����
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}
	else
	{
		//���ת��ʧ�ܣ����������Ϣ
		UE_LOG(LogTemp, Warning, TEXT("USlashAnimInstance::NativeInitializeAnimation: Failed to cast Pawn to ASlashCharacter"));
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	//��Ϊ����д�����ø���İ汾��ȷ��������κβ�������ִ��
	Super::NativeUpdateAnimation(DeltaTime);
	//����������Զ���ĸ����߼�
	//���磺���½�ɫ״̬������������
	if (SlashCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);//�����ٶȣ�
		IsFalling = SlashCharacterMovement->IsFalling(); // �Ƿ��ڿ���
		CharacterState = SlashCharacter->GetCharacterState(); // ��ȡ��ɫ״̬
	}
}
