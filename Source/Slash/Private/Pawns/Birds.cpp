// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Birds.h"
#include"Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABirds::ABirds()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//创建胶囊碰撞体组件
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));//
	Capsule->SetCapsuleHalfHeight(20.0f);//设置胶囊碰撞体的半高
	Capsule->SetCapsuleRadius(15.0f);//设置胶囊碰撞体的半径
	SetRootComponent(Capsule);//设置胶囊碰撞体组件为根组件

	BirdsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdsMesh"));//创建鸟的骨骼网格组件 也就是.h中说的子对象
	BirdsMesh->SetupAttachment(GetRootComponent());//将鸟的骨骼网格组件附加到胶囊碰撞体组件上

	
	//怎么设置输入来控制鸟的输入，移动和旋转呢？可以在蓝图中设置输入绑定，或者在C++中重写SetupPlayerInputComponent函数来绑定输入事件。

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));//创建弹簧臂组件
	SpringArm->SetupAttachment(GetRootComponent());//将弹簧臂组件附加到胶囊碰撞体组件上
	SpringArm->TargetArmLength = 300.0f;//设置弹簧臂的长度

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));//创建摄像机组件
	ViewCamera->SetupAttachment(SpringArm);//将摄像机组件附加到弹簧臂组件上

	AutoPossessPlayer = EAutoReceiveInput::Player0;//自动接收玩家输入，Player0表示第一个玩家
}

// Called when the game starts or when spawned
void ABirds::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABirds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//前进的逻辑,回调函数，需要绑定到映射轴上
void ABirds::MoveForward(float Value)
{
	//前进的逻辑
	if (Controller&&(Value != 0.0f))
	{
		//这里可以添加前进的逻辑，比如移动位置或播放动画
		FVector Forward = GetActorForwardVector();//获取前向向量并乘以输入值
		AddMovementInput(Forward,Value);//添加移动输入
		//AddActorLocalOffset(Forward, true);//添加局部偏移
	}
}

void ABirds::Turn(float Value)
{
	//转向的逻辑
	if (Controller && (Value != 0.0f))
	{
		AddControllerYawInput(Value);//添加控制器的偏航输入
	}
}

void ABirds::LookUp(float Value)
{
	//仰视的逻辑
	if (Controller && (Value != 0.0f))
	{
		AddControllerPitchInput(Value);//添加控制器的俯仰输入
	}
}

// Called to bind functionality to input
void ABirds::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);//重写了从Pawn继承的SetupPlayerInputComponent函数，

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABirds::MoveForward);//绑定输入轴到MoveForward函数
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABirds::Turn);//绑定输入轴到Turn函数
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABirds::LookUp);//绑定输入轴到LookUp函数

}

