// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
//#include"Components/CapsuleComponent.h"//胶囊碰撞体组件  不需要整个头文件，只需要声明，也就是说不需要CapsuleComponent.h中的全部代码，只需要前向声明
#include "Birds.generated.h"//这个头文件必须放在最后
class UCapsuleComponent;//前向声明，告诉编译器有这个类，不需要包含CapsuleComponent.h头文件
class USkeletalMeshComponent;//前向声明，这个类是一个骨骼网格组件，通常用于角色或生物的动画
class USpringArmComponent;//前向声明，弹簧臂组件，用于摄像机跟随物体
class UCameraComponent;//前向声明，摄像机组件，用于显示鸟的视角


UCLASS()
class SLASH_API ABirds : public APawn//在虚幻引擎中显示会去掉前缀A
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABirds();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input 玩家的输入
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);//前进
	void Turn(float Value);//转向
	void LookUp(float Value);//仰视
private:
	UPROPERTY(VisibleAnywhere)
	 UCapsuleComponent* Capsule;//胶囊碰撞体组件 前向声明 只是一个指针，还没有初始化 得在cpp中创建一个默认的子对象

	 UPROPERTY(VisibleAnywhere)
	 USkeletalMeshComponent* BirdsMesh;//鸟的骨骼网格组件，通常用于角色或生物的动画 

	 UPROPERTY(VisibleAnywhere)
	 USpringArmComponent* SpringArm;//弹簧臂组件，用于摄像机跟随物体

	 UPROPERTY(VisibleAnywhere)
	 UCameraComponent* ViewCamera;//摄像机组件，用于显示鸟的视角
};
