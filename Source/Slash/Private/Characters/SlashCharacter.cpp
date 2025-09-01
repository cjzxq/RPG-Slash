// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GroomComponent.h"
#include "Components/AttributeComponents.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"

// Sets default values 类名::构造函数名()
ASlashCharacter::ASlashCharacter()
{
	// 以下三个在蓝图details中也可以设置，在c++中设置可以更方便地控制角色
	bUseControllerRotationPitch = false; // 禁用控制器的俯仰旋转
	bUseControllerRotationYaw = false; // 启用控制器的偏航旋转
	bUseControllerRotationRoll = false; // 禁用控制器的滚转旋转	

	//访问角色移动组件
	GetCharacterMovement()->bOrientRotationToMovement = true; // 角色移动时朝向移动方向
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f); // 设置角色的旋转速率

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());//将弹簧臂组件附加到根组件
	CameraBoom->TargetArmLength = 300.0f; // 设置弹簧臂的长度

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));//
	ViewCamera->SetupAttachment(CameraBoom);//将摄像机组件附加到弹簧臂组件上

	// SetupAttachment(GetMesh())→ 蓝图中显示出现在 Mesh 下面，SetupAttachment(RootComponent) → 出现在 Root 下面
	//制作头发和眉毛
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	//需要将头发附加到角色的Mesh上，也就是网格体上。这样，Hair 组件就成为 Mesh 组件的子组件，所以蓝图的组件树中它们被显示在 Mesh 节点下面。
	Hair->SetupAttachment(GetMesh()); // 将头发组件附加到角色的网格体上，网格体是私有变量，不能直接访问，所以需要使用继承的公有GetMesh()函数获取
	//把头发附加到网格体上的插槽,不然头发不会显示在角色上
	Hair->AttachmentName = FString(TEXT("Head")); // 设置头发的附着点名称

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	//需要将眉毛附加到角色的Mesh上，也就是网格体上。这样，Hair 组件就成为 Eyebrows 组件的子组件，所以蓝图的组件树中它们被显示在 Mesh 节点下面。
	Eyebrows->SetupAttachment(GetMesh()); // 将眉毛组件附加到角色的网格体上
	Eyebrows->AttachmentName = FString(TEXT("Head")); // 设置眉毛的附着点名称
}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("EngageableTarget"));//创建标签，被敌人查询0821 在Enemy.cpp中
	//以下设置窗口部件的值
	InitializeSlashOverlay();
}

void ASlashCharacter::InitializeSlashOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());//把AController转换为APlayerController
	if (PlayerController)
	{	//为了访问HUD  角色是由PLAYERCONTROLLER控制
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD)
		{
		
			SlashOverlay = SlashHUD->GetSlashOverlay();
			if (SlashOverlay && Attributes)
			{
				SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				SlashOverlay->SetStaminaBarPercent(1.f);
				SlashOverlay->SetGold(0);
				SlashOverlay->SetSouls(0);
			}
		}
	}
}

// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent()>0.f)
	{
		ActionState = EActionState::EAS_HitReaction;//角色受到攻击时，设置为HitReaction状态
	}
}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();//更新血条
	return DamageAmount;
}

void ASlashCharacter::SetHUDHealth()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRightAndLeft"), this, &ASlashCharacter::MoveRightAndLeft);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookUp);
	//&ACharacter::Jump 和上面的不一样，这是直接调用库函数,所以没有写函数体,后面写了
	//PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);//绑定跳跃动作
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ASlashCharacter::Jump);//绑定跳跃动作
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ASlashCharacter::EkeyPressed);//
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ASlashCharacter::Attack);
}

void ASlashCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}

}

bool ASlashCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

// 需要绑定到轴映射
void ASlashCharacter::MoveForward(float Value)
{
	//如果在攻击时，就不能前进
	if (ActionState != EActionState::EAS_Unoccupied) return; // 如果正在攻击，就不允许前进 直接跳出函数
	
	//如果控制器存在并且输入值不为0，则执行前进逻辑
	if (Controller && (Value != 0.0f))
	{
		//这里可以添加前进的逻辑，比如移动位置或播放动画。为了确定哪个方向是前进方向，
		//FVector Forward = GetActorForwardVector();//获取前向向量并乘以输入值
		//AddMovementInput(Forward, Value);//添加移动输入
		//AddActorLocalOffset(Forward, true);//添加局部偏移
		const FRotator ControlRotation = GetControlRotation();//获取控制器的旋转
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);//只保留偏航旋转
		const FVector	Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//获取前向向量.控制器的旋转只影响偏航角度，
		AddMovementInput(Direction, Value);//添加移动输入
	}
}

void ASlashCharacter::MoveRightAndLeft(float Value)
{
	//如果在攻击时，就不能左右移动
	if (ActionState != EActionState::EAS_Unoccupied) return; // 如果正在攻击，就不允许左右移动 直接跳出函数

	if (Controller && (Value != 0.0f))
	{
		const FRotator ControlRotation = GetControlRotation();//获取控制器的旋转
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);//只保留偏航旋转
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);//获取右向向量
		AddMovementInput(Direction, Value);//添加移动输入
		//这里可以添加前进的逻辑，比如移动位置或播放动画
		//FVector Right = GetActorRightVector();//获取前向向量并乘以输入值
		//AddMovementInput(Right, Value);//添加移动输入
		//AddActorLocalOffset(Forward, true);//添加局部偏移
	}
}
//需要把E键绑定到动作映射，也就是SetupPlayerInputComponent 在与武器重叠时才附加武器 需要检查是否与武器重叠，是否是武器
//E键做两件事情，
void ASlashCharacter::EkeyPressed()
{
	//先直接把重叠的物品转换为武器 需要把物品和武器的头文件包含进来 Items/Item.h" "Items/Weapons/Weapon.h"
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);//转换
	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())// 装备着武器并处于空闲状态
		{
			Disarm();
		}
		else if (CanArm())// 没有else,就不会播放武器拿到手里的动画
		{
			Arm();
		}
		
	}
}

bool ASlashCharacter::CanDisarm()// 装备着武器并处于空闲状态
{
	return ActionState == EActionState::EAS_Unoccupied &&//空闲状态
		CharacterState != ECharacterState::ECS_Unequipped; //有武器
}

bool ASlashCharacter::CanArm()
{
	return  ActionState == EActionState::EAS_Unoccupied&& // 空闲状态
		CharacterState == ECharacterState::ECS_Unequipped&&
		EquippedWeapon;
}

void ASlashCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip")); // 播放把武器放到背后的蒙太奇动画
	CharacterState = ECharacterState::ECS_Unequipped; // 设置角色状态为未装备
	ActionState = EActionState::EAS_EquippingWeapon; // 
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage(FName("Equip")); // 播放把武器从背上拿到手里的蒙太奇动画
	CharacterState = ECharacterState::ECS_EquippedOneHandWeapon; // 设置角色状态为装备了一把武器
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket")); // 将武器附加到背部插槽
	}

}

void ASlashCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket")); // 将武器附加到右手插槽
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied; // 设置动作状态为空闲
}

void ASlashCharacter::HitRecatEnd()
{
	ActionState = EActionState::EAS_Unoccupied;//命中受击打动画完成时重置状态
}


void ASlashCharacter::Turn(float Value)
{
	if (Controller && (Value != 0.0f))
	{
		//这里可以添加转向的逻辑，比如旋转角色
		AddControllerYawInput(Value);
	}
}

void ASlashCharacter::LookUp(float Value)
{
	if (Controller && (Value != 0.0f))
	{
		//这里可以添加仰视的逻辑，比如旋转角色
		AddControllerPitchInput(Value);
	}
}

//攻击函数，绑定到鼠标左键按下事件 当我们需要访问动画实例时，可以通过角色网格体来获取，
void ASlashCharacter::Attack()
{
	Super::Attack();
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking; // 设置动作状态为攻击中 只攻击一次，怎么切换为EActionState::EAS_Unoccupied状态呢？void ASlashCharacter::AttackEnd()
	}

}

bool ASlashCharacter::CanAttack()// 不在攻击状态且装备了武器才能攻击
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

void ASlashCharacter::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}


/*
void ASlashCharacter::PlayAttackMontage()
{
	Super::PlayAttackMontage();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();//获取角色网格体的动画实例 获取角色 Skeletal Mesh 的动画实例（UAnimInstance）。
	if (AnimInstance && AttackMontage)//AttackMontage 是提前在类中设置的攻击动画资源（类型是 UAnimMontage*）
	{
		//如果动画实例和攻击蒙太奇都存在，就播放攻击动画 
		AnimInstance->Montage_Play(AttackMontage);//播放攻击蒙太奇，速度为1.0f
		//蒙太奇攻击动画有两个，随机跳到播放一个
		const int32 Section = FMath::RandRange(0, 1);//生成0到1之间的随机整数
		FName SectionName = FName();//生成攻击动画的名称,
		switch (Section)
		{
		case 0:
			SectionName = FName("Attack1"); // 攻击动画1 把SectionName设为一个FName类型的变量 其中Attack1名字一定要与AK_AttackMontage的节名一致
			break;
		case 1:
			SectionName = FName("Attack2"); // 攻击动画2
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);//跳转到攻击动画的某一节,AttackMontage要在蓝图中设置
	}
}
*/
void ASlashCharacter::PlayEquipMontage(const FName& SectionName)//用constFName& SectionName比FName SectionName好
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);//播放装备蒙太奇
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}


void ASlashCharacter::EquipWeapon(AWeapon*Weapon)
{
	//如果重叠的物品是武器，就装备武器 与武器重叠时按下E键装备武器 把武器的网格体附加到角色的右手插槽上
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);//装备武器到右手插槽
	//设置角色状态为装备了一把武器，让蓝图知道角色状态改变了，然后就可以改变姿势了 idle状态和拿着武器的动画不同
	CharacterState = ECharacterState::ECS_EquippedOneHandWeapon; // 
	OverlappingItem = nullptr; // 清除重叠的物品  没有这一行按下E键会没有反应
	EquippedWeapon = Weapon; // 设置当前装备的武器为重叠的武器
}

//只需将动作状态设置为空闲状态即可 在蓝图中调用这个函数
void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied; // 设置动作状态为空闲
}


