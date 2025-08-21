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
	//����������ײ�����
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));//
	Capsule->SetCapsuleHalfHeight(20.0f);//���ý�����ײ��İ��
	Capsule->SetCapsuleRadius(15.0f);//���ý�����ײ��İ뾶
	SetRootComponent(Capsule);//���ý�����ײ�����Ϊ�����

	BirdsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdsMesh"));//������Ĺ���������� Ҳ����.h��˵���Ӷ���
	BirdsMesh->SetupAttachment(GetRootComponent());//����Ĺ�������������ӵ�������ײ�������

	
	//��ô��������������������룬�ƶ�����ת�أ���������ͼ����������󶨣�������C++����дSetupPlayerInputComponent�������������¼���

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));//�������ɱ����
	SpringArm->SetupAttachment(GetRootComponent());//�����ɱ�������ӵ�������ײ�������
	SpringArm->TargetArmLength = 300.0f;//���õ��ɱ۵ĳ���

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));//������������
	ViewCamera->SetupAttachment(SpringArm);//�������������ӵ����ɱ������

	AutoPossessPlayer = EAutoReceiveInput::Player0;//�Զ�����������룬Player0��ʾ��һ�����
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

//ǰ�����߼�,�ص���������Ҫ�󶨵�ӳ������
void ABirds::MoveForward(float Value)
{
	//ǰ�����߼�
	if (Controller&&(Value != 0.0f))
	{
		//����������ǰ�����߼��������ƶ�λ�û򲥷Ŷ���
		FVector Forward = GetActorForwardVector();//��ȡǰ����������������ֵ
		AddMovementInput(Forward,Value);//����ƶ�����
		//AddActorLocalOffset(Forward, true);//��Ӿֲ�ƫ��
	}
}

void ABirds::Turn(float Value)
{
	//ת����߼�
	if (Controller && (Value != 0.0f))
	{
		AddControllerYawInput(Value);//��ӿ�������ƫ������
	}
}

void ABirds::LookUp(float Value)
{
	//���ӵ��߼�
	if (Controller && (Value != 0.0f))
	{
		AddControllerPitchInput(Value);//��ӿ������ĸ�������
	}
}

// Called to bind functionality to input
void ABirds::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);//��д�˴�Pawn�̳е�SetupPlayerInputComponent������

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABirds::MoveForward);//�������ᵽMoveForward����
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABirds::Turn);//�������ᵽTurn����
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABirds::LookUp);//�������ᵽLookUp����

}

