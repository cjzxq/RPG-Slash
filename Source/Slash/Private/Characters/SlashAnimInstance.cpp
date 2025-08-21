// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	//因为是重写，调用父类的版本，确保父类的任何操作都能执行
	Super::NativeInitializeAnimation();

	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());//把Pawn转换为SlashCharacter，
	if (SlashCharacter)
	{
		//获取角色的移动组件
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}
	else
	{
		//如果转换失败，输出错误信息
		UE_LOG(LogTemp, Warning, TEXT("USlashAnimInstance::NativeInitializeAnimation: Failed to cast Pawn to ASlashCharacter"));
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	//因为是重写，调用父类的版本，确保父类的任何操作都能执行
	Super::NativeUpdateAnimation(DeltaTime);
	//在这里添加自定义的更新逻辑
	//例如：更新角色状态、动画参数等
	if (SlashCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);//地面速度，
		IsFalling = SlashCharacterMovement->IsFalling(); // 是否在空中
		CharacterState = SlashCharacter->GetCharacterState(); // 获取角色状态
	}
}
