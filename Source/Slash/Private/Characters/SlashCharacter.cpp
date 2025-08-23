// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

// Sets default values ����::���캯����()
ASlashCharacter::ASlashCharacter()
{
	// ������������ͼdetails��Ҳ�������ã���c++�����ÿ��Ը�����ؿ��ƽ�ɫ
	bUseControllerRotationPitch = false; // ���ÿ������ĸ�����ת
	bUseControllerRotationYaw = false; // ���ÿ�������ƫ����ת
	bUseControllerRotationRoll = false; // ���ÿ������Ĺ�ת��ת	

	//���ʽ�ɫ�ƶ����
	GetCharacterMovement()->bOrientRotationToMovement = true; // ��ɫ�ƶ�ʱ�����ƶ�����
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f); // ���ý�ɫ����ת����

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());//�����ɱ�������ӵ������
	CameraBoom->TargetArmLength = 300.0f; // ���õ��ɱ۵ĳ���

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));//
	ViewCamera->SetupAttachment(CameraBoom);//�������������ӵ����ɱ������

	// SetupAttachment(GetMesh())�� ��ͼ����ʾ������ Mesh ���棬SetupAttachment(RootComponent) �� ������ Root ����
	//����ͷ����üë
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	//��Ҫ��ͷ�����ӵ���ɫ��Mesh�ϣ�Ҳ�����������ϡ�������Hair ����ͳ�Ϊ Mesh ������������������ͼ������������Ǳ���ʾ�� Mesh �ڵ����档
	Hair->SetupAttachment(GetMesh()); // ��ͷ��������ӵ���ɫ���������ϣ���������˽�б���������ֱ�ӷ��ʣ�������Ҫʹ�ü̳еĹ���GetMesh()������ȡ
	//��ͷ�����ӵ��������ϵĲ��,��Ȼͷ��������ʾ�ڽ�ɫ��
	Hair->AttachmentName = FString(TEXT("Head")); // ����ͷ���ĸ��ŵ�����

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	//��Ҫ��üë���ӵ���ɫ��Mesh�ϣ�Ҳ�����������ϡ�������Hair ����ͳ�Ϊ Eyebrows ������������������ͼ������������Ǳ���ʾ�� Mesh �ڵ����档
	Eyebrows->SetupAttachment(GetMesh()); // ��üë������ӵ���ɫ����������
	Eyebrows->AttachmentName = FString(TEXT("Head")); // ����üë�ĸ��ŵ�����
}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("SlashCharacter"));//������ǩ�������˲�ѯ0821 ��Enemy.cpp��
}

// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRightAndLeft"), this, &ASlashCharacter::MoveRightAndLeft);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookUp);
	//&ACharacter::Jump ������Ĳ�һ��������ֱ�ӵ��ÿ⺯��,����û��д������
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);//����Ծ����

	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ASlashCharacter::EkeyPressed);//
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ASlashCharacter::Attack);
}

// ��Ҫ�󶨵���ӳ��
void ASlashCharacter::MoveForward(float Value)
{
	//����ڹ���ʱ���Ͳ���ǰ��
	if (ActionState != EActionState::EAS_Unoccupied) return; // ������ڹ������Ͳ�����ǰ�� ֱ����������
	
	//������������ڲ�������ֵ��Ϊ0����ִ��ǰ���߼�
	if (Controller && (Value != 0.0f))
	{
		//����������ǰ�����߼��������ƶ�λ�û򲥷Ŷ�����Ϊ��ȷ���ĸ�������ǰ������
		//FVector Forward = GetActorForwardVector();//��ȡǰ����������������ֵ
		//AddMovementInput(Forward, Value);//����ƶ�����
		//AddActorLocalOffset(Forward, true);//��Ӿֲ�ƫ��
		const FRotator ControlRotation = GetControlRotation();//��ȡ����������ת
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);//ֻ����ƫ����ת
		const FVector	Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//��ȡǰ������.����������תֻӰ��ƫ���Ƕȣ�
		AddMovementInput(Direction, Value);//����ƶ�����
	}
}

void ASlashCharacter::MoveRightAndLeft(float Value)
{
	//����ڹ���ʱ���Ͳ��������ƶ�
	if (ActionState != EActionState::EAS_Unoccupied) return; // ������ڹ������Ͳ����������ƶ� ֱ����������

	if (Controller && (Value != 0.0f))
	{
		const FRotator ControlRotation = GetControlRotation();//��ȡ����������ת
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);//ֻ����ƫ����ת
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);//��ȡ��������
		AddMovementInput(Direction, Value);//����ƶ�����
		//����������ǰ�����߼��������ƶ�λ�û򲥷Ŷ���
		//FVector Right = GetActorRightVector();//��ȡǰ����������������ֵ
		//AddMovementInput(Right, Value);//����ƶ�����
		//AddActorLocalOffset(Forward, true);//��Ӿֲ�ƫ��
	}
}
//��Ҫ��E���󶨵�����ӳ�䣬Ҳ����SetupPlayerInputComponent ���������ص�ʱ�Ÿ������� ��Ҫ����Ƿ��������ص����Ƿ�������
//E�����������飬
void ASlashCharacter::EkeyPressed()
{
	//��ֱ�Ӱ��ص�����Ʒת��Ϊ���� ��Ҫ����Ʒ��������ͷ�ļ��������� Items/Item.h" "Items/Weapons/Weapon.h"
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);//ת��
	if (OverlappingWeapon)
	{
		//����ص�����Ʒ����������װ������ �������ص�ʱ����E��װ������ �������������帽�ӵ���ɫ�����ֲ����
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"),this,this);//װ�����������ֲ��
		//���ý�ɫ״̬Ϊװ����һ������������ͼ֪����ɫ״̬�ı��ˣ�Ȼ��Ϳ��Ըı������� idle״̬�����������Ķ�����ͬ
		CharacterState = ECharacterState::ECS_EquippedOneHandWeapon; // 
		OverlappingItem = nullptr; // ����ص�����Ʒ  û����һ�а���E����û�з�Ӧ
		EquippedWeapon = OverlappingWeapon; // ���õ�ǰװ��������Ϊ�ص�������

	}
	else
	{
		if (CanDisarm())// װ�������������ڿ���״̬
		{
			PlayEquipMontage(FName("Unequip")); // ���Ű������ŵ��������̫�涯��
			CharacterState = ECharacterState::ECS_Unequipped; // ���ý�ɫ״̬Ϊδװ��
			ActionState = EActionState::EAS_EquippingWeapon; // 
		}
		else if (CanArm())// û��else,�Ͳ��Ქ�������õ�����Ķ���
		{
			PlayEquipMontage(FName("Equip")); // ���Ű������ӱ����õ��������̫�涯��
			CharacterState = ECharacterState::ECS_EquippedOneHandWeapon; // ���ý�ɫ״̬Ϊװ����һ������
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		
	}
}



bool ASlashCharacter::CanDisarm()// װ�������������ڿ���״̬
{
	return ActionState == EActionState::EAS_Unoccupied &&//����״̬
		CharacterState != ECharacterState::ECS_Unequipped; //������
}

bool ASlashCharacter::CanArm()
{
	return  ActionState == EActionState::EAS_Unoccupied&& // ����״̬
		CharacterState == ECharacterState::ECS_Unequipped&&
		EquippedWeapon;
}

void ASlashCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket")); // ���������ӵ��������
	}

}

void ASlashCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket")); // ���������ӵ����ֲ��
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied; // ���ö���״̬Ϊ����
}


void ASlashCharacter::Turn(float Value)
{
	if (Controller && (Value != 0.0f))
	{
		//����������ת����߼���������ת��ɫ
		AddControllerYawInput(Value);
	}
}

void ASlashCharacter::LookUp(float Value)
{
	if (Controller && (Value != 0.0f))
	{
		//�������������ӵ��߼���������ת��ɫ
		AddControllerPitchInput(Value);
	}
}

//�����������󶨵������������¼� ��������Ҫ���ʶ���ʵ��ʱ������ͨ����ɫ����������ȡ��
void ASlashCharacter::Attack()
{

	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking; // ���ö���״̬Ϊ������ ֻ����һ�Σ���ô�л�ΪEActionState::EAS_Unoccupied״̬�أ�void ASlashCharacter::AttackEnd()
	}

}

bool ASlashCharacter::CanAttack()// ���ڹ���״̬��װ�����������ܹ���
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}




void ASlashCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();//��ȡ��ɫ������Ķ���ʵ�� ��ȡ��ɫ Skeletal Mesh �Ķ���ʵ����UAnimInstance����
	if (AnimInstance && AttackMontage)//AttackMontage ����ǰ���������õĹ���������Դ�������� UAnimMontage*��
	{
		//�������ʵ���͹�����̫�涼���ڣ��Ͳ��Ź������� 
		AnimInstance->Montage_Play(AttackMontage);//���Ź�����̫�棬�ٶ�Ϊ1.0f
		//��̫�湥�������������������������һ��
		const int32 Section = FMath::RandRange(0, 1);//����0��1֮����������
		FName SectionName = FName();//���ɹ�������������,
		switch (Section)
		{
		case 0:
			SectionName = FName("Attack1"); // ��������1 ��SectionName��Ϊһ��FName���͵ı��� ����Attack1����һ��Ҫ��AK_AttackMontage�Ľ���һ��
			break;
		case 1:
			SectionName = FName("Attack2"); // ��������2
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);//��ת������������ĳһ��,AttackMontageҪ����ͼ������
	}
}

void ASlashCharacter::PlayEquipMontage(const FName& SectionName)//��constFName& SectionName��FName SectionName��
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);//����װ����̫��
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}


//ֻ�轫����״̬����Ϊ����״̬���� ����ͼ�е����������
void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied; // ���ö���״̬Ϊ����
}


