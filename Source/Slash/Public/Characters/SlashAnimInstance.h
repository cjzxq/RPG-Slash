// Fill out your copyright notice in the Description page of Project Settings.
//������ͼ�Ļ����� UAnimInstance ��Echo ���ģ�� ������ ASlashCharacter ��ʹ�õĽ�ɫģ�ͣ�������ͼͨ�� Cast ��ʶ��Ͷ�ȡ��Ľ�ɫ�����ݣ��Ӷ����ƶ��������߼�����
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterType.h"
#include "SlashAnimInstance.generated.h"//���ͷ�ļ������������

/**
 * 
 */
UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// ����д������麯��
	// ��һ�������¼���ͼ��ʼ������һ��
	virtual void NativeInitializeAnimation() override;
	// �ڶ��������¼���ͼ���¶���һ��
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ASlashCharacter* SlashCharacter;//ǰ�������ڶ����÷�

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* SlashCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;//
	UPROPERTY(BlueprintReadOnly, Category = "Movement | CharacterState")
	ECharacterState CharacterState;//��ɫ״̬����ʼ״̬Ϊδװ��

};
