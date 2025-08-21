// Fill out your copyright notice in the Description page of Project Settings.
//动画蓝图的基类是 UAnimInstance 「Echo 这个模型 是你在 ASlashCharacter 中使用的角色模型，动画蓝图通过 Cast 来识别和读取你的角色类数据，从而控制动画播放逻辑。」
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterType.h"
#include "SlashAnimInstance.generated.h"//这个头文件必须在最后面

/**
 * 
 */
UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// 先重写父类的虚函数
	// 第一个，像事件蓝图初始化动画一样
	virtual void NativeInitializeAnimation() override;
	// 第二个，像事件蓝图更新动画一样
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ASlashCharacter* SlashCharacter;//前向声明第二种用法

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* SlashCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;//
	UPROPERTY(BlueprintReadOnly, Category = "Movement | CharacterState")
	ECharacterState CharacterState;//角色状态，初始状态为未装备

};
